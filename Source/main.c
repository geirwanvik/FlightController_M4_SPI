#include "main.h"

#define UpdateRateIMU 		5000 		// 200Hz
#define UpdateRatePID 		5000 		// 200Hz
#define UpdateRateSerial 	5000 		// 200Hz
#define UpdateRateRC 			20000 	// 50Hz
#define UpdateRateARM 		100000	// 10Hz

settings_t settings;
status_t status;
loopTime_t loopTime;
int main(void)
{
	InitSysTick();
	InitSerial();
	InitIMU();
	InitMotor();
	InitRC();
	
	
	InitLED(Green);
	InitLED(Red);
	InitLED(Blue);
	InitLED(Orange);
	InitPB();
	
	
	CreateTask(UpdateRateIMU,ComputeIMU);
	CreateTask(UpdateRatePID,ComputePID);
	CreateTask(UpdateRateSerial,TelegramStateMachine);
	CreateTask(UpdateRateRC,ComputeRC);
	CreateTask(UpdateRateARM,ArmCheck);

	
	RunScheduler();
}

void ArmCheck(void)
{
	LEDToggle(Blue); // Alive
	
	static int8_t armCounter;

	if(rc.throttle < 1050 && rc.yaw > 1950 && status.armed == false)
	{ // Arm function
		armCounter++;
		if(armCounter > 99) // If throttle low and yaw right for 1/50Hz * 100 = 2 sec
		{
			status.armed = true;
			LEDOn(Red);
		}
	}
	else if(rc.throttle < 1050 && rc.yaw < 1050 && status.armed == true)
	{ // Disarm function
		armCounter--;
		if(armCounter < -99) // If throttle low and yaw left for 1/50Hz * 100 = 2 sec
		{
			status.armed = false;
			LEDOff(Red);
		}
	}
	else
		armCounter = 0;
}
