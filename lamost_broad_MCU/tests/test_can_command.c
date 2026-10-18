#include "can_command.h"
#include "just_led.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

uint32_t test_primask;
static uint16_t led_mask;
static CAN_RX_Message_t incoming;
static CAN_HandleTypeDef test_can;

void just_led_control(uint16_t led_id, uint8_t state) {
  assert(led_id < 16U && state <= 1U);
  assert(test_primask == 1U);
  if (state) led_mask |= (uint16_t)(1U << led_id);
  else led_mask &= (uint16_t)~(1U << led_id);
}

int HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t fifo,
                        CAN_RxHeaderTypeDef *header, uint8_t *data) {
  assert(hcan == &test_can && fifo == CAN_RX_FIFO0);
  header->StdId = incoming.id;
  header->ExtId = incoming.id;
  header->IDE = incoming.ide ? CAN_ID_EXT : CAN_ID_STD;
  header->RTR = incoming.rtr ? CAN_RTR_REMOTE : 0U;
  header->DLC = incoming.dlc;
  memcpy(data, incoming.data, 8);
  return HAL_OK;
}

static CAN_RX_Message_t select_unit(uint8_t unit) {
  CAN_RX_Message_t msg = {0};
  msg.id = CAN_COMMAND_EXAMPLE_ID;
  msg.dlc = 4;
  msg.data[0] = 0x4C;
  msg.data[1] = 0x44;
  msg.data[2] = 1;
  msg.data[3] = unit;
  return msg;
}

int main(void) {
  CAN_RX_Handle_t rx;
  CAN_RX_Message_t msg = select_unit(TEST_LOCAL_ID), result;
  const uint16_t tens_mask = (uint16_t)(1U << (TEST_TENS_LED - 1U));
  const uint16_t ones_mask = (uint16_t)(1U << (TEST_ONES_LED - 1U));

  CAN_Command_Init(100U);
  assert(led_mask == tens_mask && can_command_status.pcb_id == PCB_ID);
  assert(can_command_status.boot_display == 1U);
  assert(can_command_status.map_ready == TEST_MAP_READY);
  CAN_Command_Poll(2099U);
  assert(led_mask == tens_mask);
  CAN_Command_Poll(2100U);
  assert(led_mask == 0U && can_command_status.boot_display);
  CAN_Command_Poll(3099U);
  assert(led_mask == 0U);
  CAN_Command_Poll(3100U);
  assert(led_mask == ones_mask);
  CAN_Command_Poll(5099U);
  assert(led_mask == ones_mask);
  CAN_Command_Poll(5100U);
  assert(led_mask == 0U && !can_command_status.boot_display);
  CAN_Command_Poll(7000U);
  assert(led_mask == 0U);

  // 计时跨越HAL tick回绕，仍在5秒时结束。
  CAN_Command_Init(UINT32_MAX - 100U);
  CAN_Command_Poll(1898U);
  assert(led_mask == tens_mask);
  CAN_Command_Poll(1899U);
  assert(led_mask == 0U);
  CAN_Command_Poll(2899U);
  assert(led_mask == ones_mask);
  CAN_Command_Poll(4898U);
  assert(can_command_status.boot_display);
  CAN_Command_Poll(4899U);
  assert(!can_command_status.boot_display && led_mask == 0U);

  // 板号显示期间收帧，不抢占板号；结束后应用最后一条合法指令。
  CAN_Command_Init(0U);
  CAN_Command_Process(&msg);
  assert(led_mask == tens_mask);
  CAN_Command_Poll(2000U);
  assert(led_mask == 0U);
  CAN_Command_Poll(3000U);
  assert(led_mask == ones_mask);
  CAN_Command_Poll(5000U);
#if CAN_COMMAND_EXAMPLE_ENABLE && TEST_MAP_READY
  assert(led_mask == (1U << TEST_LOCAL_PORT));
  CAN_Command_Process(&msg);
  assert(led_mask == (1U << TEST_LOCAL_PORT));

  // 非法或无关报文不得清除已有定位。
  for (uint8_t dlc = 0; dlc <= 9; dlc++) {
    if (dlc == 4U) continue;
    CAN_RX_Message_t bad = msg;
    bad.dlc = dlc;
    CAN_Command_Process(&bad);
    assert(led_mask == (1U << TEST_LOCAL_PORT));
  }
  CAN_RX_Message_t bad = msg;
  bad.ide = 1; CAN_Command_Process(&bad);
  bad = msg; bad.rtr = 1; CAN_Command_Process(&bad);
  bad = msg; bad.id++; CAN_Command_Process(&bad);
  bad = msg; bad.data[0] = 0; CAN_Command_Process(&bad);
  bad = msg; bad.data[2] = 9; CAN_Command_Process(&bad);
  bad = msg; bad.data[3] = 0; CAN_Command_Process(&bad);
  bad = msg; bad.data[3] = 33; CAN_Command_Process(&bad);
  assert(led_mask == (1U << TEST_LOCAL_PORT));

  // 新目标在另一块板时，本板清除旧灯。
  bad = select_unit(TEST_OTHER_ID);
  CAN_Command_Process(&bad);
  assert(led_mask == 0U);
  CAN_Command_Process(&msg);
  bad = msg; bad.data[2] = 0; bad.data[3] = 0;
  CAN_Command_Process(&bad);
  assert(led_mask == 0U && can_command_status.target_unit_id == 0U);
#else
  assert(led_mask == 0U);
#endif

  // 真实接收队列 -> 解析 -> LED输出，验证两种IDE和回绕后的消息次序。
  CAN_RX_Init(&rx, &test_can);
  incoming = msg;
  HAL_CAN_RxFifo0MsgPendingCallback(&test_can);
  CAN_RX_Process(&rx);
#if CAN_COMMAND_EXAMPLE_ENABLE && TEST_MAP_READY
  assert(led_mask == (1U << TEST_LOCAL_PORT));
#else
  assert(led_mask == 0U);
#endif
  for (uint32_t round = 0; round < 3; round++) {
    for (uint32_t i = 0; i < CAN_RX_BUFFER_SIZE; i++) {
      incoming.id = i;
      incoming.ide = (uint8_t)(i & 1U);
      HAL_CAN_RxFifo0MsgPendingCallback(&test_can);
    }
    assert(rx.overflow_count == round + 1U);
    for (uint32_t i = 0; i < CAN_RX_BUFFER_SIZE - 1U; i++) {
      assert(CAN_RX_Read(&rx, &result));
      assert(result.id == i && result.ide == (i & 1U));
    }
    assert(!CAN_RX_Read(&rx, &result));
  }
  test_primask = 1;
  CAN_Command_Init(0U);
  assert(test_primask == 1U);
  assert(led_mask == tens_mask);
  puts("PASS: board display, timing, parser, LED selection and CAN queue");
  return 0;
}
