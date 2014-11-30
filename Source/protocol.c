#include "protocol.h"

static ProtocolFrame inFrame;

bool ReceiveFrame(void)
{
	static ProtocolStates state = Start;
	
	while (SerialAvailable())
	{
		uint8_t data = SerialReadByte();
		
		switch(state)
		{
			case Start:
				state = CaseStart(data);
				break;
				
			case Direction:
				state = CaseDirection(data);
				break;
					
			case Command:
				state = CaseCommand(data);
				break;
				
			case Length:
				state = CaseLength(data);
				break;
				
			case Payload:
				state = CasePayload(data);
				if(state == Payload)
				{ break; }
				
			case Crc:	
				state = CaseCRC(data);
				if(state == Crc)
				{ break; }
					
			case End:
				state = Start;
				if(inFrame.crc == GetCRC())
				{
					return true; // process telegram	
				}
				break;
		}
	}
	return false; // not finished & buffer empty
}

ProtocolStates CaseStart(uint8_t data)
{
	if(data == StartByte)
	{
		inFrame.start = data;
		ClearCRC();
		UpdateCRC(data);
		return Direction; // next
	}
	return Start; // stay
}

ProtocolStates CaseDirection(uint8_t data)
{
	inFrame.direction = data;
	UpdateCRC(data);
	return Command; // next
}

ProtocolStates CaseCommand(uint8_t data)
{
	inFrame.command = data;
	UpdateCRC(data);
	return Length; // next
}

ProtocolStates CaseLength(uint8_t data)
{
	if(data > 64) // length not valid
	{
		return Start; // goto start
	}
	else
	{
		inFrame.length = data;
		UpdateCRC(data);
		return Payload; // next
	}
}

ProtocolStates CasePayload(uint8_t data)
{
	static uint8_t plCounter = 0;
	if(plCounter == inFrame.length)
	{
		plCounter = 0;
		return Crc; // next, no break
	}
	else
	{
		inFrame.payload[plCounter++]=data;
		UpdateCRC(data);
		return Payload; // stay
	}
}

ProtocolStates CaseCRC(uint8_t data)
{
	static uint8_t crcCounter = 0;
	if(crcCounter == 0)
	{
		inFrame.crc = data;
		crcCounter++;
		return Crc; //stay
	}
	else if(crcCounter == 1)
	{
		inFrame.crc = inFrame.crc | (data << 8);
		crcCounter = 0;
	}
	return End; // next, no break
}

void GetFrameData(uint8_t* command, uint8_t* direction, uint8_t* payload, uint8_t* length)
{
	*command = inFrame.command;
	*direction = inFrame.direction;
	*length = inFrame.length;
	memcpy(payload,&inFrame.payload,inFrame.length);
}

void SendFrame(uint8_t command, uint8_t direction, uint8_t* payload, uint8_t length)
{
	ClearCRC();
	
	SerialWriteByte(StartByte);
	UpdateCRC(StartByte);
	
	SerialWriteByte(direction);
	UpdateCRC(direction);
	
	SerialWriteByte(command);
	UpdateCRC(command);
	
	SerialWriteByte(length);
	UpdateCRC(length);
	
	for(uint8_t i = 0; i< length; i++)
	{
		SerialWriteByte(payload[i]);
		UpdateCRC(payload[i]);
	}
	
	uint16_t crc = GetCRC();
	
	SerialWriteByte(crc & 0xFF);
	SerialWriteByte(crc >> 8);
}

