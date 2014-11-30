#include "time.h"

volatile uint32_t timeDelay = 0;
volatile uint32_t counter = 0;
volatile uint32_t pbFilterCounter = 0;
void InitSysTick(void)
{
	//System clock = 168MHz / 1MHz = 168 counts pr IRQ == 1uS resolution.
	if (SysTick_Config(SystemCoreClock / 1000000))
	{ 
		/* Capture error */ 
		while (1);
  }
}	

void SysTick_Handler(void)
{
		if(timeDelay != 0x00)
			timeDelay--;
		if(pbFilterCounter != 0x00)
			pbFilterCounter--;
		counter++;
}

void DelayMicros(__IO uint32_t nTime)
{
  timeDelay = nTime;

  while(timeDelay != 0);
}

void DelayMillis(__IO uint32_t nTime)
{ 
  timeDelay = nTime*1000;

  while(timeDelay != 0);
}


uint32_t Micros(void)
{
    return counter;
}

uint32_t Millis(void)
{
	return (counter / 1000);
}
