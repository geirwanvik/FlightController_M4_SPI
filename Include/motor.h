#ifndef MOTOR_H
#define MOTOR_H

#include "stm32f4xx.h"
#include "pinout.h"
#include "commandlibrary.h"
#include <stdbool.h>

PUBLIC void InitMotor(void);
PUBLIC void MotorUpdate(void);

#endif
