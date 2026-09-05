#include "just_led.h"
#include "main.h"


// 外部单元映射：数组下标 0~15 对应 USB1/LED1 ~ USB16/LED16
// 数组值 N 表示 Canvas 单元 AN，0 表示该端口未使用
#if PCB_ID == 1
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    8U, 1U, 9U, 21U, 22U, 10U, 23U, 11U
};
#elif PCB_ID == 2
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    2U, 3U, 25U, 4U, 28U, 29U, 5U, 6U,
    24U, 12U, 26U, 27U, 13U, 48U, 14U, 30U
};
#elif PCB_ID == 3
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U,
    15U, 31U, 16U, 32U, 17U, 7U, 18U, 0U
};
#elif PCB_ID == 4
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 0U, 57U, 58U,
    0U, 0U, 0U, 0U, 0U, 82U, 83U, 84U
};
#elif PCB_ID == 5
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    36U, 37U, 19U, 38U, 61U, 20U, 39U, 62U,
    85U, 59U, 111U, 86U, 60U, 87U, 113U, 88U
};
#elif PCB_ID == 6
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    40U, 41U, 64U, 42U, 65U, 43U, 66U, 44U,
    63U, 89U, 115U, 90U, 116U, 117U, 91U, 118U
};
#elif PCB_ID == 7
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    67U, 68U, 45U, 46U, 69U, 47U, 70U, 71U,
    92U, 93U, 120U, 94U, 121U, 122U, 95U, 96U
};
#elif PCB_ID == 8
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    49U, 72U, 50U, 73U, 51U, 74U, 33U, 52U,
    124U, 97U, 125U, 126U, 98U, 127U, 99U, 75U
};
#elif PCB_ID == 9
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    76U, 53U, 34U, 77U, 54U, 35U, 55U, 56U,
    100U, 129U, 101U, 78U, 102U, 131U, 79U, 103U
};
#elif PCB_ID == 10
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    80U, 81U, 0U, 0U, 0U, 0U, 0U, 0U,
    104U, 105U, 106U, 0U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 11
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 136U, 166U, 137U, 107U,
    0U, 0U, 0U, 194U, 165U, 195U, 226U, 196U
};
#elif PCB_ID == 12
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    138U, 168U, 139U, 109U, 140U, 110U, 170U, 141U,
    167U, 197U, 108U, 198U, 169U, 229U, 199U, 200U
};
#elif PCB_ID == 13
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    142U, 112U, 172U, 143U, 114U, 144U, 174U, 145U,
    171U, 201U, 232U, 202U, 173U, 233U, 203U, 234U
};
#elif PCB_ID == 14
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    175U, 146U, 176U, 147U, 177U, 119U, 178U, 149U,
    204U, 205U, 236U, 206U, 237U, 148U, 207U, 208U
};
#elif PCB_ID == 15
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    150U, 180U, 151U, 152U, 181U, 153U, 154U, 183U,
    179U, 209U, 123U, 241U, 210U, 182U, 211U, 212U
};
#elif PCB_ID == 16
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    155U, 184U, 156U, 128U, 157U, 186U, 130U, 158U,
    244U, 213U, 245U, 185U, 214U, 246U, 215U, 187U
};
#elif PCB_ID == 17
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    159U, 188U, 132U, 160U, 133U, 161U, 190U, 162U,
    216U, 217U, 249U, 189U, 218U, 134U, 219U, 191U
};
#elif PCB_ID == 18
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    135U, 163U, 192U, 164U, 0U, 0U, 0U, 0U,
    220U, 252U, 221U, 193U, 222U, 0U, 0U, 0U
};
#elif PCB_ID == 19
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    286U, 256U, 223U, 287U, 224U, 258U, 288U, 225U,
    338U, 312U, 339U, 313U, 257U, 340U, 314U, 341U
};
#elif PCB_ID == 20
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    289U, 259U, 260U, 290U, 227U, 261U, 228U, 291U,
    315U, 342U, 316U, 343U, 317U, 344U, 318U, 345U
};
#elif PCB_ID == 21
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    262U, 230U, 263U, 293U, 231U, 264U, 294U, 265U,
    319U, 346U, 292U, 320U, 347U, 321U, 348U, 322U
};
#elif PCB_ID == 22
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    295U, 266U, 296U, 235U, 268U, 269U, 238U, 270U,
    349U, 350U, 323U, 267U, 324U, 297U, 298U, 239U
};
#elif PCB_ID == 23
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    271U, 240U, 300U, 273U, 243U, 301U, 275U, 302U,
    299U, 272U, 242U, 325U, 274U, 326U, 351U, 352U
};
#elif PCB_ID == 24
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    276U, 303U, 277U, 247U, 304U, 278U, 248U, 279U,
    327U, 353U, 328U, 354U, 329U, 305U, 355U, 330U
};
#elif PCB_ID == 25
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    306U, 250U, 280U, 251U, 307U, 281U, 282U, 308U,
    356U, 331U, 357U, 332U, 358U, 333U, 359U, 334U
};
#elif PCB_ID == 26
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    253U, 309U, 283U, 254U, 310U, 255U, 285U, 311U,
    360U, 335U, 361U, 284U, 336U, 362U, 337U, 363U
};
#elif PCB_ID == 27
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    364U, 390U, 365U, 391U, 443U, 366U, 392U, 367U,
    416U, 442U, 472U, 417U, 473U, 444U, 418U, 474U
};
#elif PCB_ID == 28
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    393U, 368U, 394U, 369U, 395U, 370U, 396U, 371U,
    419U, 445U, 446U, 420U, 476U, 447U, 477U, 421U
};
#elif PCB_ID == 29
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    397U, 372U, 422U, 398U, 373U, 399U, 374U, 400U,
    448U, 479U, 449U, 423U, 480U, 450U, 424U, 451U
};
#elif PCB_ID == 30
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    375U, 376U, 401U, 453U, 402U, 427U, 428U, 488U,
    425U, 452U, 426U, 484U, 454U, 455U, 487U, 456U
};
#elif PCB_ID == 31
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    429U, 458U, 491U, 403U, 460U, 404U, 377U, 378U,
    457U, 489U, 430U, 459U, 492U, 431U, 461U, 432U
};
#elif PCB_ID == 32
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    405U, 379U, 406U, 380U, 407U, 435U, 381U, 408U,
    462U, 433U, 463U, 496U, 434U, 464U, 497U, 465U
};
#elif PCB_ID == 33
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    382U, 409U, 383U, 410U, 384U, 411U, 385U, 412U,
    436U, 499U, 466U, 500U, 437U, 467U, 468U, 438U
};
#elif PCB_ID == 34
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    386U, 413U, 387U, 470U, 414U, 388U, 415U, 389U,
    502U, 439U, 469U, 503U, 440U, 504U, 471U, 441U
};
#elif PCB_ID == 35
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 505U, 534U, 506U, 475U, 507U,
    0U, 0U, 0U, 0U, 563U, 535U, 564U, 592U
};
#elif PCB_ID == 36
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    536U, 508U, 593U, 509U, 538U, 478U, 510U, 511U,
    565U, 537U, 566U, 594U, 567U, 595U, 539U, 568U
};
#elif PCB_ID == 37
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    540U, 512U, 481U, 513U, 542U, 482U, 514U, 483U,
    569U, 597U, 541U, 570U, 599U, 571U, 543U, 572U
};
#elif PCB_ID == 38
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    515U, 516U, 485U, 517U, 486U, 575U, 518U, 519U,
    544U, 573U, 545U, 574U, 546U, 604U, 547U, 576U
};
#elif PCB_ID == 39
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    548U, 520U, 608U, 490U, 521U, 551U, 522U, 523U,
    577U, 549U, 578U, 579U, 550U, 580U, 581U, 552U
};
#elif PCB_ID == 40
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    493U, 524U, 494U, 554U, 525U, 495U, 526U, 556U,
    582U, 553U, 583U, 613U, 584U, 555U, 615U, 585U
};
#elif PCB_ID == 41
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    527U, 528U, 498U, 558U, 529U, 619U, 530U, 560U,
    586U, 557U, 617U, 587U, 618U, 588U, 559U, 589U
};
#elif PCB_ID == 42
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    531U, 501U, 532U, 562U, 533U, 0U, 0U, 0U,
    620U, 590U, 561U, 591U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 43
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    0U, 0U, 0U, 0U, 0U, 621U, 622U, 623U,
    0U, 0U, 0U, 0U, 0U, 0U, 646U, 647U
};
#elif PCB_ID == 44
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    624U, 648U, 596U, 625U, 649U, 626U, 598U, 627U,
    671U, 672U, 692U, 673U, 650U, 693U, 674U, 651U
};
#elif PCB_ID == 45
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    652U, 628U, 600U, 629U, 601U, 602U, 630U, 603U,
    675U, 676U, 653U, 677U, 654U, 678U, 655U, 679U
};
#elif PCB_ID == 46
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    631U, 632U, 605U, 633U, 606U, 607U, 634U, 635U,
    656U, 657U, 680U, 681U, 658U, 682U, 659U, 660U
};
#elif PCB_ID == 47
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    609U, 636U, 610U, 611U, 637U, 612U, 638U, 664U,
    684U, 661U, 685U, 662U, 686U, 663U, 706U, 687U
};
#elif PCB_ID == 48
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    639U, 614U, 640U, 667U, 641U, 616U, 668U, 642U,
    665U, 688U, 707U, 666U, 689U, 708U, 690U, 691U
};
#elif PCB_ID == 49
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    643U, 644U, 645U, 0U, 0U, 0U, 0U, 0U,
    669U, 670U, 0U, 0U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 50
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    709U, 720U, 710U, 694U, 695U, 711U, 696U, 712U,
    0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U
};
#elif PCB_ID == 51
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    697U, 713U, 699U, 700U, 714U, 683U, 715U, 703U,
    721U, 722U, 698U, 723U, 701U, 702U, 724U, 725U
};
#elif PCB_ID == 52
const uint16_t pcb_map[JUST_BOARD_LED_NUMBER] = {
    716U, 704U, 717U, 705U, 718U, 726U, 719U, 0U,
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






