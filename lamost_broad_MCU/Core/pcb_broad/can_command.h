#ifndef __CAN_COMMAND_H
#define __CAN_COMMAND_H

#include "can_rx_buffer.h"

// 开机先显示十位2秒，熄灭1秒，再显示个位2秒；LED10表示数字0
#define CAN_COMMAND_DIGIT_TIME_MS 2000U
#define CAN_COMMAND_DIGIT_GAP_MS 1000U
#define CAN_COMMAND_BOOT_TIME_MS (2U * CAN_COMMAND_DIGIT_TIME_MS + CAN_COMMAND_DIGIT_GAP_MS)

// 正式报文格式尚未确定。示例协议仅用于台架验证，默认关闭。
#ifndef CAN_COMMAND_EXAMPLE_ENABLE
#define CAN_COMMAND_EXAMPLE_ENABLE 0
#endif
#define CAN_COMMAND_EXAMPLE_ID 0x6F0U

// 按本板实际接线填写16个组内单元ID（1~32），0表示未配置。
// 此表不是全局焦面 pcb_map，也不根据 PCB_ID 自动推断单元编号。
#ifndef CAN_COMMAND_PORT_UNIT_IDS
#define CAN_COMMAND_PORT_UNIT_IDS {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
#endif

// 本地调试状态，不通过CAN发送
typedef struct {
  uint32_t received_count;
  uint32_t command_count;
  uint32_t invalid_count;
  uint32_t ignored_count;
  uint32_t local_count;
  uint32_t other_count;
  uint16_t led_mask;
  uint8_t pcb_id;
  uint8_t target_unit_id;
  uint8_t boot_display;
  uint8_t map_ready;
  uint8_t protocol_enabled;
} CAN_CommandStatus_t;

extern volatile CAN_CommandStatus_t can_command_status;

void CAN_Command_Init(uint32_t now_ms);
void CAN_Command_Process(const CAN_RX_Message_t *msg);
void CAN_Command_Poll(uint32_t now_ms);

#endif
