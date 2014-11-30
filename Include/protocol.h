/*
* Protocol.h
*
* Created: 07.11.2014 10:09:11
* Author: GeirW
*/


#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "stm32f4xx.h"
#include "pinout.h"
#include "checksum.h"
#include <string.h>
#include "serial.h"

typedef enum { Start, Direction, Command, Length, Payload, Crc, End } ProtocolStates;
enum Identifier { StartByte = '#' };

typedef struct { uint8_t start; uint8_t direction; uint8_t command; uint8_t length;
						uint8_t payload[64]; uint16_t crc; } ProtocolFrame;

PUBLIC bool ReceiveFrame(void);
PUBLIC void GetFrameData(uint8_t* command, uint8_t* direction, uint8_t* payload, uint8_t* length);
PUBLIC void SendFrame(uint8_t command, uint8_t direction, uint8_t* payload, uint8_t length);

PRIVATE ProtocolStates CaseStart(uint8_t data);
PRIVATE ProtocolStates CaseDirection(uint8_t data);
PRIVATE ProtocolStates CaseCommand(uint8_t data);
PRIVATE ProtocolStates CaseLength(uint8_t data);
PRIVATE ProtocolStates CasePayload(uint8_t data);
PRIVATE ProtocolStates CaseCRC(uint8_t data);
						
#endif //__PROTOCOL_H__
