#include "pid.h"

p_t p;
motor_t motor;

int16_t rollP, rollI = 0, rollD, rollPID;
int16_t pitchP, pitchI = 0, pitchD, pitchPID;
int16_t yawP, yawI = 0, yawD, yawPID;
int16_t lastRollError = 0, lastPitchError = 0, lastYawError = 0;

void ComputePID(void)
{

	// ROLL
	int16_t error = (rc.roll - 1500);
	error -= imu.gyroADC[ROLL];
	rollP = error * p.rollPID.kP;
	rollI += error;
	rollI = p.rollPID.kI * constrain(rollI,-p.iMax,p.iMax);
	rollD = p.rollPID.kD * (error - lastRollError);
	lastRollError = error;
	rollPID = rollP + rollI + rollD;
	
	// PITCH
	error = (rc.pitch - 1500);
	error -= imu.gyroADC[PITCH];
	pitchP = error * p.pitchPID.kP;
	pitchI += error;
	pitchI = p.pitchPID.kI * constrain(pitchI,-p.iMax,p.iMax);
	pitchD = p.pitchPID.kD * (error - lastPitchError);
	lastPitchError = error;
	pitchPID = pitchP + pitchI + pitchD;
	
	// YAW
	error = (rc.yaw - 1500);
	error -= imu.gyroADC[YAW];
	yawP = error * p.yawPID.kP;
	yawI += error;
	yawI = p.yawPID.kI * constrain(yawI,-p.iMax,p.iMax);
	yawD = p.yawPID.kD * (error - lastYawError);
	lastYawError = error;
	yawPID = yawP + yawI + yawD;
	

	motor.back = rc.throttle + pitchPID - yawPID;
	motor.right = rc.throttle - rollPID + yawPID;
	motor.left = rc.throttle + rollPID + yawPID;
	motor.front = rc.throttle - pitchPID -yawPID;
	
	MotorUpdate();
	
	// Motor Mix - multiwii quad+ - burde stemme med gyro/imu aksene
	
	// back = throttle + pitchPID - yawPID
	
	// right = throttle - rollPID + yawPID
	
	// left = throttle + rollPID + yawPID
	
	// front = throttle - pitchPID - yawPID
	
	
}
