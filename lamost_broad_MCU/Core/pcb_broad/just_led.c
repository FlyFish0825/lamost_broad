#include "just_led.h"
#include "main.h"


const uint16_t pcb_1_map[16] = {0x0201, 0x0202, 0x0203, 0x0101, 0x0204, 0x0102,
                                0x0205, 0x0103, 0x0301, 0x0302, 0x0303, 0x0304,
                                0x0305, 0x0306, 0x0307, 0x0308};




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) {
        // 在这里处理 TIM1 的周期性中断事件
        // 例如，切换 LED 状态或执行其他定时任务
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); // 点亮LED
    }
}                                

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) {
        // 在这里处理 TIM1 的输出比较中断事件
        // 例如，切换 LED 状态或执行其他定时任务
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); // 熄灭LED
    }
}

