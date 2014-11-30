#ifndef SENSORS_H
#define SENSORS_H

#include "stm32f4xx.h"
#include "pinout.h"
#include "spi.h"
#include "time.h"
#include "commandlibrary.h"
#include "utilities.h"

PUBLIC void InitGyroAcc(void);
PUBLIC void GetGyroData(int16_t* gyroData);
PUBLIC void GetAccData(int16_t* accData);

PRIVATE void ReadRawGyro(int16_t* data);
PRIVATE void ReadRawAcc(int16_t* data);

#endif
