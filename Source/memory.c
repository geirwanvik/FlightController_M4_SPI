#include "memory.h"

// Based on AQ32Plus functions

const int StartingAddress = 0x08004000; // Flash sector 1 from programming manual

char myVerify = 101; // Random number for checking EEPROM Flash config after power on.

void ReadEEPROM(void)
{
	memcpy(&settings, (char *)StartingAddress, sizeof(settings_t));
}

void WriteEEPROM(void)
{
	uint8_t eepromBuffer[sizeof(settings_t)];
	FLASH_Status status;
	
	memcpy(eepromBuffer,&settings,sizeof(settings_t));
	FLASH_Unlock();
	
	FLASH_ClearFlag(FLASH_FLAG_EOP    | FLASH_FLAG_OPERR  | FLASH_FLAG_WRPERR |
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	
	if(FLASH_EraseSector(FLASH_Sector_1, VoltageRange_3) == FLASH_COMPLETE)
	{
		for(short i = 0; i < sizeof(settings_t); i++)
		{
			status = FLASH_ProgramByte(StartingAddress + i, *(eepromBuffer +i));
			if(status != FLASH_COMPLETE)
			{
				break;
			}
		}
	}
	FLASH_Lock();
}

void ValidateEEPROM(void)
{
	char EepromVerify;
	
	EepromVerify = *(char*)StartingAddress;
	
	if(EepromVerify != myVerify) // No config in flash, load default values
	{
		WriteEEPROM();
	}
	ReadEEPROM();
}
