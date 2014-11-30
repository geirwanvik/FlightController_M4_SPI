#ifndef SPI_H
#define SPI_H

#include "stm32f4xx.h"
#include "pinout.h"

inline void SetCSPin() {GPIOE->BSRRH = GPIO_Pin_7;}
inline void ResetCSPin(){GPIOE->BSRRL = GPIO_Pin_7;}

PUBLIC void InitSPI(void);
PUBLIC void SetSPIClockDivider(uint16_t divider);
PUBLIC void ReadBytesSPI(uint8_t* buffer, uint8_t length, uint8_t reg);
PUBLIC uint8_t ReadByteSPI(uint8_t reg);
PUBLIC void WriteByteSPI(uint8_t reg, uint8_t data);

PRIVATE uint8_t Transfer(uint8_t data);

#endif
