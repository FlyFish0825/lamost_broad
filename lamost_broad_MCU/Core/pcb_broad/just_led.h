#ifndef __JUST_LED_H
#define __JUST_LED_H

#include "main.h"

typedef struct LED{
    GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t state; // 0: off, 1: on, 2: blinking
}PCB_Broad_LED;


extern const uint16_t pcb_1_map[16];
extern  PCB_Broad_LED pcb_leds[16];


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);





void just_led_control(uint16_t led_id, uint8_t state);




#endif /* __JUST_LED_H */
