//
// Created by sh956 on 2023/9/10.
//

#ifndef GUI_DEMO_TIMER_H
#define GUI_DEMO_TIMER_H

#include "main.h"

extern TIM_HandleTypeDef TIM3_Handler;      //定时器句柄

void TIM3_Init(uint16_t arr,uint16_t sc);

#endif //GUI_DEMO_TIMER_H
