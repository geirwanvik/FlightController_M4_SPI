#ifndef TIME_H
#define TIME_H

#include "stm32f4xx.h"
#include "pinout.h"

PUBLIC void InitSysTick(void);
PUBLIC void DelayMicros(__IO uint32_t nTime);
PUBLIC void DelayMillis(__IO uint32_t nTime);
PUBLIC uint32_t Micros(void);
PUBLIC uint32_t Millis(void);

volatile extern uint32_t pbFilterCounter;

#endif
