#ifndef CHECKSUM_H
#define CHECKSUM_H

#include "stm32f4xx.h"
#include "pinout.h"
#include <stdbool.h>

PUBLIC void UpdateCRC(uint8_t data);
PUBLIC void ClearCRC(void);
PUBLIC uint16_t GetCRC(void);

#endif // CHECKSUM_H
