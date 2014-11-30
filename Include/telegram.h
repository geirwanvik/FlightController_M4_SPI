/* 
* Telegram.h
*
* Created: 09.11.2014 12:03:26
* Author: GeirW
*/


#ifndef __TELEGRAM_H__
#define __TELEGRAM_H__

#include "stm32f4xx.h"
#include "pinout.h"
#include <string.h>

#include "protocol.h"
#include "commandlibrary.h"

PUBLIC void TelegramStateMachine(void);

PRIVATE void ProcessTelegram(void);
PRIVATE void SendTelegram(void);
PRIVATE void SendSensorData(void);
PRIVATE void SendAttData(void);
PRIVATE void SendPidData(void);
PRIVATE void StorePidData(uint8_t* payload, uint8_t length);
PRIVATE void SendRcData(void);
PRIVATE void SendEepromData(void);
PRIVATE void StoreEepromData(uint8_t* payload, uint8_t length);
PRIVATE void SendMotorData(void);
PRIVATE void StoreStatus(uint8_t* payload, uint8_t length);
PRIVATE void SendStatus(void);
PRIVATE void SendLoopTime(void);
#endif //__TELEGRAM_H__
