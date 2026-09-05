#ifndef __CAN_RX_BUFFER_H
#define __CAN_RX_BUFFER_H

#include "can.h"
#include "main.h"

#define CAN_RX_BUFFER_SIZE 32

/*
 * CAN接收消息
 *
 * 压缩版，不保存HAL的CAN_RxHeaderTypeDef
 */
typedef struct {

  /*
   * CAN ID
   */
  uint32_t id;

  /*
   * 标志位
   */
  union {

    uint8_t flags;

    struct {

      /*
       * 0 标准帧
       * 1 扩展帧
       */
      uint8_t ide : 1;

      /*
       * 0 数据帧
       * 1 远程帧
       */
      uint8_t rtr : 1;

      uint8_t reserve : 6;
    };
  };

  /*
   * 数据长度
   */
  uint8_t dlc;

  /*
   * 数据
   */
  uint8_t data[8];

} CAN_RX_Message_t;

/*
 * CAN接收控制结构
 */
typedef struct {

  /*
   * CAN句柄
   */
  CAN_HandleTypeDef *hcan;

  /*
   * 环形缓存
   */
  CAN_RX_Message_t buffer[CAN_RX_BUFFER_SIZE];

  /*
   * 写指针
   */
  volatile uint16_t write_index;

  /*
   * 读指针
   */
  volatile uint16_t read_index;

  /*
   * 溢出计数
   */
  volatile uint32_t overflow_count;

  /*
   * 当前缓存数量
   *
   * 仅用于调试观察
   */
  volatile uint16_t count;

} CAN_RX_Handle_t;

/*
 * 初始化
 */
void CAN_RX_Init(CAN_RX_Handle_t *rx, CAN_HandleTypeDef *hcan);

/*
 * HAL CAN回调
 *
 * 不需要用户调用
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

/*
 * 读取缓存
 */
uint8_t CAN_RX_Read(CAN_RX_Handle_t *rx, CAN_RX_Message_t *msg);

/*
 * 主循环处理
 */
void CAN_RX_Process(CAN_RX_Handle_t *rx);

#endif