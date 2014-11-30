#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "stm32f4xx.h"
#include <stdlib.h>
#include "pinout.h"
#include "time.h"

typedef struct
{
	uint32_t interval;
	uint32_t lastTick;
	void (*Func)(void);
} TaskType;

PUBLIC void RunScheduler(void);
PUBLIC void CreateTask(uint32_t interval, void (*f)(void));


#endif
