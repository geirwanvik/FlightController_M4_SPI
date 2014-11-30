#ifndef LEDBUTTON_H
#define LEDBUTTON_H

#include "stm32f4xx.h"
#include "pinout.h"
#include "time.h"

typedef enum 
{
	Green = 0,
	Orange = 1,
	Red = 2,
	Blue = 3,
	NumLeds = 4
}LEDColors;

PUBLIC void InitLED(LEDColors color);
PUBLIC void LEDOn(LEDColors color);
PUBLIC void LEDOff(LEDColors color);
PUBLIC void LEDToggle(LEDColors color);

PUBLIC void InitPB(void);

volatile extern int8_t pbPressed;


#endif
