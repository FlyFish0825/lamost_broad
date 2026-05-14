#include "just_led.h"
#include "main.h"


const uint16_t pcb_1_map[16] = {0x0201, 0x0202, 0x0203, 0x0101, 0x0204, 0x0102,
                                0x0205, 0x0103, 0x0301, 0x0302, 0x0303, 0x0304,
                                0x0305, 0x0306, 0x0307, 0x0308};

// LED mapping: LED1..LED16
PCB_Broad_LED pcb_leds[16] = {
    {GPIOA, GPIO_PIN_0, 0},  // LED1 PA0
    {GPIOA, GPIO_PIN_1, 0},  // LED2 PA1
    {GPIOA, GPIO_PIN_2, 0},  // LED3 PA2
    {GPIOA, GPIO_PIN_3, 0},  // LED4 PA3
    {GPIOA, GPIO_PIN_4, 0},  // LED5 PA4
    {GPIOA, GPIO_PIN_5, 0},  // LED6 PA5
    {GPIOA, GPIO_PIN_6, 0},  // LED7 PA6
    {GPIOA, GPIO_PIN_7, 0},  // LED8 PA7
    {GPIOA, GPIO_PIN_8, 0},  // LED9 PA8
    {GPIOB, GPIO_PIN_0, 0},  // LED10 PB0
    {GPIOA, GPIO_PIN_15, 0}, // LED11 PA15
    {GPIOB, GPIO_PIN_3, 0},  // LED12 PB3
    {GPIOB, GPIO_PIN_4, 0},  // LED13 PB4
    {GPIOB, GPIO_PIN_5, 0},  // LED14 PB5
    {GPIOB, GPIO_PIN_6, 0},  // LED15 PB6
    {GPIOB, GPIO_PIN_7, 0},  // LED16 PB7
};

CAN_RxHeaderTypeDef   RxHeader;
uint8_t               RxData[8];     // 假设最大数据长度 8 字节
uint8_t               RxFlag = 0;    // 收到新数据的标志




void just_led_light(uint16_t led_id) {
  if (led_id < JUST_BOARD_LED_NUMBER) {
    PCB_Broad_LED PCB_LED = pcb_leds[led_id];
    HAL_GPIO_WritePin(PCB_LED.port, PCB_LED.pin, 1); // 点亮LED
  }
}

void just_led_Delight(uint16_t led_id) {
  if (led_id < JUST_BOARD_LED_NUMBER) {
    PCB_Broad_LED PCB_LED = pcb_leds[led_id];
    HAL_GPIO_WritePin(PCB_LED.port, PCB_LED.pin, 0); // 熄灭LED
  }
}



void just_led_control(uint16_t led_id, uint8_t state)
{
    pcb_leds[led_id].state = state; // 更新LED状态
}




void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  static uint8_t blink_counter = 0; // 溢出中断计数器
  static uint8_t blink_phase = 1;   // 当前亮灭状态：1-亮，0-灭
  if (htim->Instance == TIM1) {
    // 在这里处理 TIM1 的周期性中断事件
    // 例如，切换 LED 状态或执行其他定时任务
    // ------ 闪烁节拍控制（每进一次溢出中断计一次） ------
        blink_counter++;
        if (blink_counter >= 50) {      // 26 → 闪烁频率约 2 Hz
            blink_counter = 0;
            blink_phase = !blink_phase;
        }

      // 遍历所有 LED，根据各自状态决定是否点亮
      for (uint8_t i = 0; i < JUST_BOARD_LED_NUMBER; i++) {
        uint8_t state = pcb_leds[i].state;
        uint8_t should_light = 0;

        if (state == 1) {
          should_light = 1; // 常亮
        } else if (state == 2) {
          should_light = blink_phase; // 闪烁，跟随全局相位
        } // state == 0 则 should_light 保持 0，不点亮

        if (should_light) {
          just_led_light(i); // 进入 PWM 点亮周期（之后由 OC 中断熄灭）
        }
      }
    }
  }


void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {

  if (htim->Instance == TIM1) {
    // 在这里处理 TIM1 的输出比较中断事件
    // 例如，切换 LED 状态或执行其他定时任务
    for (uint8_t i = 0; i < JUST_BOARD_LED_NUMBER; i++) {
      just_led_Delight(i);
    }
  }
}



void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* 从 FIFO0 中取出消息 */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
        RxFlag = 1;   // 告诉主循环有新数据到了
        // 这里不要做耗时操作！
    }
}





