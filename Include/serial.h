#ifndef SERIAL_H
#define SERIAL_H

#include "stm32f4xx.h"
#include "pinout.h"
#include <stdio.h>


PUBLIC void InitSerial(void);
PUBLIC uint16_t SerialAvailable(void);
PUBLIC uint8_t SerialReadByte(void);
PUBLIC void SerialWriteByte(uint8_t);
PUBLIC void SerialWriteBytes(uint8_t* buffer, uint8_t length);
PRIVATE void TransmittData(void);


#endif
