#ifndef MEMORY_H
#define MEMORY_H

#include "stm32f4xx.h"
#include "commandlibrary.h"
#include <stdlib.h>

extern void* memcpy(void*, const void*, size_t); // To avoid compiler warning, implicit declaration of memcpy.

void ReadEEPROM(void);
void WriteEEPROM(void);
void ValidateEEPROM(void);

#endif
