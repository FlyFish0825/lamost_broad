#include "just_led.h"
#include "main.h"


// 外部单元映射：数组下标 0~15 对应 USB1/LED1 ~ USB16/LED16
// 数组值 N 表示 Canvas 单元 AN，0 表示该端口未使用
#if PCB_ID == 1
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    221U, 281U, 249U, 248U, 278U, 252U, 280U, 279U
};
#elif PCB_ID == 2
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    305U, 329U, 301U, 374U, 396U, 420U, 421U, 445U,
    304U, 328U, 351U, 373U, 352U, 419U, 397U, 444U
};
#elif PCB_ID == 3
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    471U, 473U, 502U, 472U, 499U, 474U, 500U, 0U
};
#elif PCB_ID == 4
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 0U, 77U, 98U,
    0U, 0U, 0U, 0U, 0U, 75U, 76U, 142U
};
#elif PCB_ID == 5
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    121U, 168U, 170U, 146U, 145U, 220U, 171U, 219U,
    97U, 99U, 143U, 144U, 122U, 167U, 169U, 195U
};
#elif PCB_ID == 6
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    197U, 224U, 223U, 251U, 250U, 303U, 302U, 327U,
    196U, 222U, 194U, 247U, 276U, 275U, 277U, 300U
};
#elif PCB_ID == 7
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    326U, 349U, 350U, 372U, 371U, 395U, 394U, 418U,
    325U, 348U, 347U, 370U, 369U, 392U, 393U, 417U
};
#elif PCB_ID == 8
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    443U, 442U, 501U, 470U, 529U, 528U, 498U, 555U,
    440U, 441U, 468U, 469U, 497U, 552U, 527U, 526U
};
#elif PCB_ID == 9
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    524U, 580U, 554U, 604U, 579U, 553U, 603U, 602U,
    525U, 578U, 576U, 627U, 577U, 626U, 649U, 600U
};
#elif PCB_ID == 10
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    648U, 670U, 0U, 0U, 0U, 0U, 0U, 0U,
    625U, 647U, 646U, 0U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 11
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 71U, 73U, 22U, 54U,
    0U, 0U, 0U, 72U, 21U, 35U, 114U, 36U
};
#elif PCB_ID == 12
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    139U, 74U, 55U, 96U, 117U, 140U, 164U, 119U,
    37U, 38U, 93U, 56U, 94U, 213U, 95U, 118U
};
#elif PCB_ID == 13
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    120U, 190U, 166U, 218U, 192U, 193U, 244U, 246U,
    141U, 165U, 215U, 216U, 191U, 242U, 217U, 243U
};
#elif PCB_ID == 14
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    273U, 274U, 298U, 299U, 322U, 324U, 345U, 346U,
    245U, 297U, 319U, 321U, 320U, 323U, 344U, 366U
};
#elif PCB_ID == 15
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    368U, 390U, 391U, 415U, 414U, 439U, 467U, 437U,
    367U, 364U, 416U, 388U, 413U, 438U, 412U, 466U
};
#elif PCB_ID == 16
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    496U, 495U, 551U, 550U, 523U, 521U, 575U, 601U,
    493U, 494U, 548U, 549U, 522U, 520U, 574U, 599U
};
#elif PCB_ID == 17
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    624U, 623U, 598U, 622U, 669U, 687U, 688U, 667U,
    573U, 645U, 596U, 644U, 668U, 686U, 597U, 703U
};
#elif PCB_ID == 18
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    666U, 702U, 704U, 717U, 0U, 0U, 0U, 0U,
    665U, 642U, 701U, 716U, 643U, 0U, 0U, 0U
};
#elif PCB_ID == 19
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    49U, 8U, 9U, 18U, 20U, 112U, 33U, 34U,
    16U, 17U, 6U, 7U, 19U, 48U, 89U, 31U
};
#elif PCB_ID == 20
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    50U, 91U, 113U, 51U, 70U, 92U, 137U, 52U,
    32U, 67U, 135U, 68U, 69U, 111U, 136U, 90U
};
#elif PCB_ID == 21
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    240U, 53U, 162U, 115U, 241U, 188U, 116U, 214U,
    186U, 160U, 161U, 138U, 185U, 212U, 211U, 187U
};
#elif PCB_ID == 22
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    270U, 163U, 189U, 271U, 272U, 296U, 342U, 343U,
    239U, 267U, 268U, 269U, 294U, 295U, 318U, 365U
};
#elif PCB_ID == 23
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    389U, 387U, 436U, 410U, 464U, 547U, 572U, 492U,
    411U, 465U, 435U, 463U, 434U, 462U, 461U, 489U
};
#elif PCB_ID == 24
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    519U, 621U, 546U, 491U, 620U, 571U, 685U, 619U,
    545U, 517U, 490U, 543U, 518U, 570U, 569U, 544U
};
#elif PCB_ID == 25
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    664U, 595U, 684U, 641U, 663U, 618U, 683U, 682U,
    640U, 594U, 616U, 662U, 661U, 593U, 639U, 698U
};
#elif PCB_ID == 26
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    700U, 699U, 617U, 715U, 713U, 726U, 725U, 681U,
    680U, 724U, 723U, 714U, 712U, 711U, 697U, 696U
};
#elif PCB_ID == 27
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    5U, 4U, 30U, 29U, 12U, 15U, 3U, 47U,
    14U, 13U, 1U, 28U, 86U, 2U, 27U, 11U
};
#elif PCB_ID == 28
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    46U, 88U, 110U, 66U, 65U, 134U, 133U, 87U,
    45U, 64U, 109U, 26U, 44U, 43U, 157U, 85U
};
#elif PCB_ID == 29
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    158U, 159U, 183U, 131U, 184U, 237U, 210U, 209U,
    108U, 42U, 182U, 107U, 236U, 155U, 106U, 181U
};
#elif PCB_ID == 30
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    238U, 266U, 265U, 293U, 264U, 207U, 262U, 362U,
    208U, 180U, 317U, 292U, 341U, 291U, 363U, 340U
};
#elif PCB_ID == 31
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    408U, 432U, 409U, 433U, 458U, 459U, 460U, 488U,
    385U, 386U, 431U, 455U, 456U, 538U, 457U, 486U
};
#elif PCB_ID == 32
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    540U, 516U, 515U, 542U, 589U, 541U, 568U, 567U,
    513U, 611U, 539U, 487U, 612U, 565U, 674U, 613U
};
#elif PCB_ID == 33
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    637U, 591U, 592U, 658U, 659U, 615U, 660U, 678U,
    675U, 590U, 656U, 657U, 692U, 614U, 676U, 677U
};
#elif PCB_ID == 34
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    695U, 638U, 721U, 693U, 709U, 710U, 679U, 722U,
    706U, 707U, 719U, 636U, 694U, 718U, 708U, 720U
};
#elif PCB_ID == 35
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 23U, 10U, 25U, 132U, 62U,
    0U, 0U, 0U, 0U, 63U, 83U, 61U, 24U
};
#elif PCB_ID == 36
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    41U, 60U, 84U, 129U, 130U, 156U, 82U, 104U,
    39U, 59U, 40U, 58U, 105U, 128U, 154U, 103U
};
#elif PCB_ID == 37
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    206U, 153U, 235U, 205U, 178U, 179U, 233U, 263U,
    126U, 152U, 234U, 204U, 177U, 176U, 232U, 231U
};
#elif PCB_ID == 38
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    261U, 290U, 316U, 314U, 315U, 312U, 339U, 361U,
    260U, 259U, 289U, 288U, 313U, 311U, 338U, 337U
};
#elif PCB_ID == 39
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    360U, 384U, 403U, 407U, 406U, 429U, 430U, 482U,
    359U, 383U, 382U, 404U, 405U, 428U, 453U, 454U
};
#elif PCB_ID == 40
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    484U, 510U, 485U, 536U, 511U, 512U, 562U, 586U,
    481U, 483U, 534U, 535U, 509U, 561U, 537U, 584U
};
#elif PCB_ID == 41
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    563U, 610U, 514U, 588U, 671U, 634U, 564U, 673U,
    608U, 609U, 631U, 632U, 651U, 672U, 652U, 653U
};
#elif PCB_ID == 42
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    655U, 635U, 566U, 705U, 689U, 0U, 0U, 0U,
    690U, 654U, 633U, 691U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 43
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 81U, 80U, 102U,
    0U, 0U, 0U, 0U, 0U, 0U, 57U, 79U
};
#elif PCB_ID == 44
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    127U, 78U, 101U, 150U, 100U, 151U, 149U, 202U,
    125U, 124U, 174U, 148U, 123U, 173U, 147U, 203U
};
#elif PCB_ID == 45
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    201U, 200U, 175U, 230U, 257U, 258U, 286U, 287U,
    172U, 198U, 199U, 226U, 256U, 284U, 285U, 308U
};
#elif PCB_ID == 46
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    310U, 335U, 336U, 357U, 358U, 381U, 380U, 402U,
    309U, 334U, 333U, 355U, 356U, 378U, 379U, 401U
};
#elif PCB_ID == 47
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    427U, 426U, 452U, 451U, 480U, 533U, 505U, 506U,
    424U, 425U, 476U, 450U, 503U, 504U, 479U, 530U
};
#elif PCB_ID == 48
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    507U, 558U, 560U, 605U, 559U, 606U, 628U, 585U,
    508U, 556U, 531U, 581U, 557U, 532U, 582U, 583U
};
#elif PCB_ID == 49
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    607U, 630U, 587U, 0U, 0U, 0U, 0U, 0U,
    629U, 650U, 0U, 0U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 50
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    229U, 253U, 227U, 228U, 255U, 225U, 254U, 283U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 51
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    306U, 331U, 332U, 354U, 353U, 400U, 376U, 423U,
    282U, 330U, 307U, 375U, 377U, 399U, 398U, 422U
};
#elif PCB_ID == 52
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    448U, 447U, 475U, 449U, 478U, 446U, 477U, 0U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};
#else
#error "PCB_ID must be between 1 and 52"
#endif

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
    if (led_id < JUST_BOARD_LED_NUMBER) {
        pcb_leds[led_id].state = state; // 更新LED状态
    }
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






