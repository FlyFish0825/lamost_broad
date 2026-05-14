#ifndef __JUST_LED_H
#define __JUST_LED_H

#include "main.h"


// PCB 上 LED 的总数量
#define JUST_BOARD_LED_NUMBER 16

/**
 * @brief LED 引脚及状态结构体
 * @param port  : 所属 GPIO 端口
 * @param pin   : 引脚编号
 * @param state : 0-熄灭，1-常亮，2-闪烁
 */
typedef struct LED{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t state;
}PCB_Broad_LED;

// 外部变量声明 —— PCB1 的连接映射表（单元ID数组）
extern const uint16_t pcb_1_map[16];
// 外部变量声明 —— LED 硬件引脚状态数组
extern PCB_Broad_LED pcb_leds[16];

// TIM1 溢出中断回调，实现 LED 状态刷新
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
// TIM1 输出比较中断回调，实现 LED 熄灭控制
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);
// 设置指定 LED 的工作模式（常亮/闪烁/熄灭）
void just_led_control(uint16_t led_id, uint8_t state);

#endif /* __JUST_LED_H */