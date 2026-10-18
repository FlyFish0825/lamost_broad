#include "can_command.h"
#include "just_led.h"
#include <string.h>

static const uint8_t port_unit_ids[] = CAN_COMMAND_PORT_UNIT_IDS;
_Static_assert(sizeof(port_unit_ids) == JUST_BOARD_LED_NUMBER,
               "CAN_COMMAND_PORT_UNIT_IDS must contain 16 entries");

volatile CAN_CommandStatus_t can_command_status;
static uint32_t boot_start_ms;
static uint16_t command_led_mask;

// 数字1~9对应LED1~LED9，数字0对应LED10
static uint16_t CAN_Command_DigitMask(uint8_t digit) {
  uint8_t led_number = digit == 0U ? 10U : digit;
  return (uint16_t)(1U << (led_number - 1U));
}

// 一次更新整板状态，避免定时器中断读到新旧混合的灯状态
static void CAN_Command_SetLedMask(uint16_t mask) {
  uint32_t primask = __get_PRIMASK();
  __disable_irq();
  for (uint8_t i = 0; i < JUST_BOARD_LED_NUMBER; i++) {
    just_led_control(i, (mask & (1U << i)) ? 1U : 0U);
  }
  __set_PRIMASK(primask);
  can_command_status.led_mask = mask;
}

void CAN_Command_Init(uint32_t now_ms) {
  uint32_t used_ids = 0;
  uint8_t valid = 1;

  memset((void *)&can_command_status, 0, sizeof(can_command_status));
  boot_start_ms = now_ms;
  command_led_mask = 0;
  can_command_status.pcb_id = PCB_ID;
  can_command_status.protocol_enabled = CAN_COMMAND_EXAMPLE_ENABLE ? 1U : 0U;

  for (uint8_t i = 0; i < sizeof(port_unit_ids); i++) {
    uint8_t id = port_unit_ids[i];
    if (id == 0U) {
      continue;
    }
    if (id > 32U || (used_ids & (1UL << (id - 1U)))) {
      valid = 0;
      break;
    }
    used_ids |= 1UL << (id - 1U);
  }
  can_command_status.map_ready = valid && used_ids != 0U;
  can_command_status.boot_display = 1;
  CAN_Command_SetLedMask(CAN_Command_DigitMask(PCB_ID / 10U));
}

// 返回1：合法定位/取消指令；0：无关报文；-1：格式错误。
// 正式协议确定后，只替换本函数中的报文识别和字段提取。
static int CAN_Command_Decode(const CAN_RX_Message_t *msg, uint8_t *unit_id) {
#if CAN_COMMAND_EXAMPLE_ENABLE
  // 台架示例：标准数据帧，ID=0x6F0，DLC=4。
  // 4C 44 01 nn：定位组内单元nn（1~32）；4C 44 00 00：取消定位。
  if (msg->ide || msg->rtr || msg->id != CAN_COMMAND_EXAMPLE_ID) {
    return 0;
  }
  if (msg->dlc != 4U || msg->data[0] != 0x4CU || msg->data[1] != 0x44U) {
    return -1;
  }
  if (msg->data[2] == 0U && msg->data[3] == 0U) {
    *unit_id = 0;
    return 1;
  }
  if (msg->data[2] == 1U && msg->data[3] >= 1U && msg->data[3] <= 32U) {
    *unit_id = msg->data[3];
    return 1;
  }
  return -1;
#else
  // 未接入真实协议时只监听，不将普通CAN报文误识别为定位指令。
  (void)msg;
  (void)unit_id;
  return 0;
#endif
}

void CAN_Command_Process(const CAN_RX_Message_t *msg) {
  uint8_t unit_id = 0;
  int result;

  if (msg == NULL) {
    return;
  }
  can_command_status.received_count++;
  result = CAN_Command_Decode(msg, &unit_id);
  if (result == 0) {
    can_command_status.ignored_count++;
    return;
  }
  if (result < 0) {
    can_command_status.invalid_count++;
    return;
  }

  can_command_status.command_count++;
  can_command_status.target_unit_id = unit_id;
  command_led_mask = 0;
  if (unit_id != 0U) {
    if (can_command_status.map_ready) {
      for (uint8_t i = 0; i < sizeof(port_unit_ids); i++) {
        if (port_unit_ids[i] == unit_id) {
          command_led_mask = (uint16_t)(1U << i);
          break;
        }
      }
    }
    if (command_led_mask != 0U) {
      can_command_status.local_count++;
    } else {
      can_command_status.other_count++;
    }
  }

  // 新目标在另一块板时，本板也清除旧指示，避免同组两块板同时留灯。
  // 板号显示期间只记录最后一条合法指令，5秒结束后再应用。
  if (!can_command_status.boot_display) {
    CAN_Command_SetLedMask(command_led_mask);
  }
}

void CAN_Command_Poll(uint32_t now_ms) {
  uint32_t elapsed;
  uint16_t mask;

  if (!can_command_status.boot_display) {
    return;
  }
  elapsed = (uint32_t)(now_ms - boot_start_ms);
  if (elapsed < CAN_COMMAND_DIGIT_TIME_MS) {
    mask = CAN_Command_DigitMask(PCB_ID / 10U);
  } else if (elapsed < CAN_COMMAND_DIGIT_TIME_MS + CAN_COMMAND_DIGIT_GAP_MS) {
    mask = 0;
  } else if (elapsed < CAN_COMMAND_BOOT_TIME_MS) {
    mask = CAN_Command_DigitMask(PCB_ID % 10U);
  } else {
    can_command_status.boot_display = 0;
    mask = command_led_mask;
  }
  if (mask != can_command_status.led_mask) {
    CAN_Command_SetLedMask(mask);
  }
}
