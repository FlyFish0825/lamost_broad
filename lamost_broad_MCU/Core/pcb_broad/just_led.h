#ifndef __JUST_LED_H
#define __JUST_LED_H

#include "main.h"

extern const uint16_t pcb_1_map[16];



void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim);


#endif /* __JUST_LED_H */
