#include "can_rx_buffer.h"
#include <string.h>

/*
 * 当前CAN接收实例

 * 用于HAL回调
 */
static CAN_RX_Handle_t *can_rx = NULL;

/*
 * 内部函数声明
 */
static void CAN_RX_Write(CAN_RX_Handle_t *rx, CAN_RX_Message_t *msg);

static void CAN_RX_UpdateCount(CAN_RX_Handle_t *rx);

/*
 * 初始化CAN接收模块
 */
void CAN_RX_Init(CAN_RX_Handle_t *rx, CAN_HandleTypeDef *hcan) {

  /*
   * 清空整个结构体
   */
  memset(rx, 0, sizeof(CAN_RX_Handle_t));

  /*
   * 绑定CAN
   */
  rx->hcan = hcan;

  /*
   * 注册实例
   */
  can_rx = rx;
}

/*
 * 写入环形缓存
 */
static void CAN_RX_Write(CAN_RX_Handle_t *rx, CAN_RX_Message_t *msg) {

  uint16_t next;

  /*
   * 计算下一个写位置
   */
  next = rx->write_index + 1;

  if (next >= CAN_RX_BUFFER_SIZE) {
    next = 0;
  }

  /*
   * 判断缓存满
   */
  if (next == rx->read_index) {

    rx->overflow_count++;

    return;
  }

  /*
   * 保存数据
   */
  rx->buffer[rx->write_index] = *msg;

  /*
   * 更新写指针
   */
  rx->write_index = next;

  /*
   * 更新缓存数量
   */
  CAN_RX_UpdateCount(rx);
}

/*
 * HAL CAN FIFO0接收回调

 * CubeMX自动调用

 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {

  CAN_RxHeaderTypeDef header;

  uint8_t data[8];

  CAN_RX_Message_t msg = {0};

  /*
   * 判断实例
   */
  if (can_rx == NULL) {
    return;
  }

  if (hcan != can_rx->hcan) {
    return;
  }

  /*
   * 从CAN FIFO读取数据
   */
  if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header, data) != HAL_OK) {

    return;
  }

  /*
   * ID处理
   */
  if (header.IDE == CAN_ID_STD) {

    /*
     * 标准帧
     */
    msg.id = header.StdId;

    msg.ide = 0;

  } else {

    /*
     * 扩展帧
     */
    msg.id = header.ExtId;

    msg.ide = 1;
  }

  /*
   * RTR
   */
  msg.rtr = (header.RTR == CAN_RTR_REMOTE);

  /*
   * 数据长度
   */
  msg.dlc = header.DLC;

  /*
   * 数据复制
   */
  memcpy(msg.data, data, 8);

  /*
   * 放入环形缓存
   */
  CAN_RX_Write(can_rx, &msg);
}

/*
 * 更新当前缓存数量

 */
static void CAN_RX_UpdateCount(CAN_RX_Handle_t *rx) {

  if (rx->write_index >= rx->read_index) {

    rx->count = rx->write_index - rx->read_index;

  } else {

    rx->count = CAN_RX_BUFFER_SIZE - rx->read_index + rx->write_index;
  }
}

/*
 * 读取一帧CAN数据

 * 返回:
 * 1 有数据
 * 0 无数据

 */
uint8_t CAN_RX_Read(CAN_RX_Handle_t *rx, CAN_RX_Message_t *msg) {

  /*
   * 空
   */
  if (rx->read_index == rx->write_index) {
    return 0;
  }

  /*
   * 读取数据
   */
  *msg = rx->buffer[rx->read_index];

  /*
   * 更新读指针
   */
  rx->read_index++;

  if (rx->read_index >= CAN_RX_BUFFER_SIZE) {
    rx->read_index = 0;
  }

  /*
   * 更新数量
   */
  CAN_RX_UpdateCount(rx);

  return 1;
}

/*
 * 主循环调用

 * 后续CAN协议解析放这里

 */
void CAN_RX_Process(CAN_RX_Handle_t *rx) {

  CAN_RX_Message_t msg;

  while (CAN_RX_Read(rx, &msg)) {

    /*
     *
     * CAN协议解析区域
     *
     *
     * 示例:
     *
     * if(msg.id == 0x100)
     * {
     *
     * }
     *
     */
  }
}