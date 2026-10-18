#ifndef TEST_CAN_H
#define TEST_CAN_H
#include "main.h"
typedef struct { uint32_t unused; } CAN_HandleTypeDef;
typedef struct {
  uint32_t StdId, ExtId, IDE, RTR, DLC;
} CAN_RxHeaderTypeDef;
#define CAN_ID_STD 0U
#define CAN_ID_EXT 4U
#define CAN_RTR_REMOTE 2U
#define CAN_RX_FIFO0 0U
#define HAL_OK 0
#define HAL_ERROR 1
int HAL_CAN_GetRxMessage(CAN_HandleTypeDef *hcan, uint32_t fifo,
                        CAN_RxHeaderTypeDef *header, uint8_t *data);
#endif
