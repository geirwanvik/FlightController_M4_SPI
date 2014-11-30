#include "sensors.h"

int16_t gyroZero[3] = {0,0,0};
uint16_t calibratingA = 512;  // The calibration is done in the main loop,
uint16_t calibratingG = 512;  // calibrating decreases at each cycle down to 0, then we enter in a normal mode.

void InitGyroAcc(void)
{
	InitSPI();
	DelayMillis(50);
	
		//PWR_MGMT_1    -- DEVICE_RESET 1
	WriteByteSPI(0x6B, 0x80);
	DelayMillis(5);
	
	uint8_t regstate = ReadByteSPI(0x6A);
	WriteByteSPI(0x6A, regstate | 0x10); // SPI Mode Enabled
	DelayMillis(5);	
	
	//PWR_MGMT_1    -- SLEEP 0; CYCLE 0; TEMP_DIS 0; CLKSEL 3 (PLL with Z Gyro reference)
	WriteByteSPI(0x6B, 0x03);
	
	//CONFIG        -- DLPF_CFG = 3 => ACC bandwidth = 44Hz GYRO bandwidth = 42Hz)
	WriteByteSPI(0x1A, 0x03);
	
	//GYRO_CONFIG   -- FS_SEL = 3: Full scale set to 2000 deg/sec
	WriteByteSPI(0x1B, 0x18);

	//ACCEL_CONFIG  -- AFS_SEL=2 (Full Scale = +/-8G)  ; ACCELL_HPF=0
	WriteByteSPI(0x1C, 0x10);
	
	SetSPIClockDivider(4);
	
}

void GetGyroData(int16_t* gyroData)
{
	ReadRawGyro(gyroData);

	static int16_t previousGyroADC[3] = {0,0,0};
	static int32_t g[3];
	uint8_t axis;
	
	if (calibratingG>0)
	{
		for (axis = 0; axis < 3; axis++)
		{
			// Reset g[axis] at start of calibration
			if (calibratingG == 512)
			{
				g[axis]=0;
			}
			// Sum up 512 readings
			g[axis] +=gyroData[axis];
			// Clear global variables for next reading
			gyroData[axis]=0;
			gyroZero[axis]=0;
			if (calibratingG == 1)
			{
				gyroZero[axis]=(g[axis]+256)>>9;
			}
		}
		calibratingG--;
	}
	
	for (axis = 0; axis < 3; axis++)
	{
		gyroData[axis]  -= gyroZero[axis];
		//anti gyro glitch, limit the variation between two consecutive readings
		gyroData[axis] = constrain(gyroData[axis],previousGyroADC[axis]-800,previousGyroADC[axis]+800);
		
		previousGyroADC[axis] = gyroData[axis];
	}
}

void ReadRawGyro(int16_t* rawData)
{
	uint8_t data[6];
	
	ReadBytesSPI(data,6,0x43);

	rawData[PITCH] =  (int16_t)( ( data[0] << 8 ) | data[1] ) >> 2 ;
	rawData[ROLL] =  (-1 )* (int16_t)( ( data[2] << 8 ) | data[3] ) >> 2 ;
	rawData[YAW] = (-1) *  (int16_t)( ( data[4] << 8 ) | data[5] ) >> 2 ;
}

void GetAccData(int16_t* accData)
{
	ReadRawAcc(accData);

	static int32_t a[3];
	if(status.accDoCalibrate)
	{
		calibratingA = 512;
		status.accDoCalibrate = 0;
	}
	if (calibratingA>0)
	{
		for (uint8_t axis = 0; axis < 3; axis++)
		{
			// Reset a[axis] at start of calibration
			if (calibratingA == 512) a[axis]=0;
			// Sum up 512 readings
			a[axis] +=accData[axis];
			// Clear global variables for next reading
			accData[axis]=0;
			settings.accOffset[axis]=0;
		}
		// Calculate average, shift Z down by ACC_1G ( 512 ) and store values
		if (calibratingA == 1) {
			settings.accOffset[ROLL]  = (a[ROLL]+256)>>9;
			settings.accOffset[PITCH] = (a[PITCH]+256)>>9;
			settings.accOffset[YAW]   = ((a[YAW]+256)>>9) - ( 512 );
		}
		calibratingA--;
	}
	
	accData[ROLL]  -=  settings.accOffset[ROLL] ;
	accData[PITCH] -=  settings.accOffset[PITCH];
	accData[YAW]   -=  settings.accOffset[YAW] ;	
}

void ReadRawAcc(int16_t* rawData)
{
	uint8_t data[6];
	
	ReadBytesSPI(data,6,0x3B);
	
	rawData[ROLL] =  (int16_t)( (data[0] << 8) | data[1] ) >>3;
	rawData[PITCH] = (int16_t)( (data[2] << 8) | data[3] ) >>3;
	rawData[YAW] = (int16_t)( (data[4] << 8) | data[5] ) >>3;	
}

