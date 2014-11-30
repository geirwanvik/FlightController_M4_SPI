#include "telegram.h"

void TelegramStateMachine(void)
{
	bool processComplete = ReceiveFrame();
	
	if(processComplete)
	{
		ProcessTelegram();
	}
}

void ProcessTelegram(void)
{
	uint8_t payload[64];
	uint8_t command;
	uint8_t direction;
	uint8_t length;
	
	GetFrameData(&command,&direction,payload,&length);
	
	switch(command)
	{
		case IMU_CMD:
		SendSensorData();
		break;
		
		case ATT_CMD:
		SendAttData();
		break;
		
		case PID_CMD:
		if(direction == WriteDirection)
			StorePidData(payload,length);
		else
			SendPidData();
		break;
		
		case RC_CMD:
		SendRcData();
		break;
		
		case MOTOR_CMD:
		SendMotorData();
		break;
		
		case EEPROM_CMD:
		if (direction == WriteDirection)
			StoreEepromData(payload,length);
		else
			SendEepromData();
		break;
		
		case STATUS_CMD:
		if (direction == WriteDirection)
			StoreStatus(payload,length); 
		else
			SendStatus();
		break;
		
		case LOOPT_CMD:
		SendLoopTime();
		break;
	}
}

void SendSensorData(void)
{
	uint8_t length = sizeof(imu_t);
	uint8_t payload[length];
	uint8_t command = IMU_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&imu,length);
	SendFrame(command,direction,payload,length);
}

void SendAttData(void)
{
	uint8_t length = sizeof(att_t);
	uint8_t payload[length];
	uint8_t command = ATT_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&att,length);
	SendFrame(command,direction,payload,length);
}

void SendPidData(void)
{
	uint8_t length = sizeof(p_t);
	uint8_t payload[length];
	uint8_t command = PID_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&p,sizeof(p_t));
	SendFrame(command,direction,payload,length);
}
void StorePidData(uint8_t* payload, uint8_t length)
{
	memcpy(&p,payload,length);
	//WritePidData();
	uint8_t outPayload;
	uint8_t command = PID_CMD;
	uint8_t direction =  Ack;
	uint8_t outLength = 0;
	SendFrame(command,direction,&outPayload,outLength);
}

void SendRcData(void)
{
	uint8_t length = sizeof(rc_t);
	uint8_t payload[length];
	uint8_t command = RC_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&rc,length);
	SendFrame(command,direction,payload,length);
}

void SendMotorData(void)
{
	uint8_t length = sizeof(motor_t);
	uint8_t payload[length];
	uint8_t command = MOTOR_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&motor,length);
	SendFrame(command,direction,payload,length);
}

void StoreEepromData(uint8_t* payload, uint8_t length)
{
	memcpy(&settings,payload,length);
	//SaveMemory();
	uint8_t outPayload;
	uint8_t command = EEPROM_CMD;
	uint8_t direction =  Ack;
	uint8_t outLength = 0;
	SendFrame(command,direction,&outPayload,outLength);
}

void SendEepromData(void)
{
	uint8_t length = sizeof(settings_t);
	uint8_t payload[length];
	uint8_t command = EEPROM_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&settings,length);
	SendFrame(command,direction,payload,length);
}

void StoreStatus(uint8_t* payload, uint8_t length)
{
	memcpy(&status,payload,length);
	uint8_t outPayload;
	uint8_t command = STATUS_CMD;
	uint8_t direction = Ack;
	uint8_t outLength = 0;
	SendFrame(command,direction,&outPayload,outLength);
}

void SendStatus(void)
{
	uint8_t length = sizeof(status_t);
	uint8_t payload[length];
	uint8_t command = STATUS_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&status,length);
	SendFrame(command,direction,payload,length);
}

void SendLoopTime(void)
{
	uint8_t length = sizeof(loopTime_t);
	uint8_t payload[length];
	uint8_t command = LOOPT_CMD;
	uint8_t direction = WriteDirection;
	memcpy(payload,&loopTime,length);
	SendFrame(command,direction,payload,length);	
}
