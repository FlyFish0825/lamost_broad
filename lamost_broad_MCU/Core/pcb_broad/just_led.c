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


// CAN 接收相关全局变量
CAN_RxHeaderTypeDef   RxHeader;   // 接收到的报文头信息
uint8_t               RxData[8];  // 接收到的报文数据（最大 8 字节）
uint8_t               RxFlag = 0; // 接收标志，1 表示有新数据，需在主循环中清零



/**
 * @brief  点亮指定编号的 LED（常亮）
 * @param  led_id : LED 序号（0 ~ JUST_BOARD_LED_NUMBER-1）
 * @note   该函数直接操作 GPIO 输出高电平，需确保引脚已初始化为推挽输出
 */
void just_led_light(uint16_t led_id) {
  if (led_id < JUST_BOARD_LED_NUMBER) {
    PCB_Broad_LED PCB_LED = pcb_leds[led_id];
    HAL_GPIO_WritePin(PCB_LED.port, PCB_LED.pin, 1); // 点亮LED
  }
}


/**
 * @brief  熄灭指定编号的 LED
 * @param  led_id : LED 序号（0 ~ JUST_BOARD_LED_NUMBER-1）
 */
void just_led_Delight(uint16_t led_id) {
  if (led_id < JUST_BOARD_LED_NUMBER) {
    PCB_Broad_LED PCB_LED = pcb_leds[led_id];
    HAL_GPIO_WritePin(PCB_LED.port, PCB_LED.pin, 0); // 熄灭LED
  }
}



/**
 * @brief  设置指定 LED 的工作状态
 * @param  led_id : LED 序号
 * @param  state  : 状态值，0=熄灭，1=常亮，2=闪烁
 * @note   该函数仅更新软件状态标志，实际亮灭由定时器中断回调统一处理
 */
void just_led_control(uint16_t led_id, uint8_t state)
{
    pcb_leds[led_id].state = state; // 更新LED状态
}



/**
 * @brief  TIM1 溢出中断回调函数（实现 LED 闪烁及调光）
 * @param  htim : 指向触发中断的定时器句柄
 * @note   每发生一次溢出中断，全局计数器递增；达到阈值后翻转闪烁相位。
 *         然后遍历所有 LED，根据其 state 决定是否点亮；
 *         点亮操作会打开 LED，随后的 OC 中断负责熄灭，形成简易 PWM。
 */
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



  /**
 * @brief  TIM1 输出比较中断回调（用于熄灭所有 LED，完成 PWM 周期）
 * @param  htim : 指向触发中断的定时器句柄
 * @note   在 OC 事件发生时，强制关闭所有 LED 输出，与溢出中断配合实现调光。
 */
void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim) {

  if (htim->Instance == TIM1) {
    // 在这里处理 TIM1 的输出比较中断事件
    // 例如，切换 LED 状态或执行其他定时任务
    for (uint8_t i = 0; i < JUST_BOARD_LED_NUMBER; i++) {
      just_led_Delight(i);
    }
  }
}



/**
 * @brief  CAN FIFO0 消息挂起回调函数
 * @param  hcan : CAN 句柄指针
 * @note   当 FIFO0 中有新消息时，HAL 库在中断服务函数中调用本回调。
 *         这里仅取出报文并置位接收标志，后续由主循环处理，避免在中断中耗时。
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* 从 FIFO0 中取出消息 */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData) == HAL_OK)
    {
        RxFlag = 1;   // 告诉主循环有新数据到了
        // 这里不要做耗时操作！
    }
}





