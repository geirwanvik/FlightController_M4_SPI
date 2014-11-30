#ifndef IMU_H
#define IMU_H

#include "stm32f4xx.h"
#include "pinout.h"
#include "sensors.h"
#include "commandlibrary.h"

#define abs(x) ((x)>0?(x):-(x))
#define sq(x) ((x)*(x))
#define PI 3.1415926535897932384626433832795
#define GYRO_SCALE (4 / 16.4 * PI / 180.0 / 1000000.0)   //MPU6050 16.4 LSB/(deg/s) and we ignore the last 2 bits
#define ACC_LPF_FACTOR 4 // that means a LPF of 16
#define ACC_1G 512
#define GYR_CMPF_FACTOR 600
#define GYR_CMPFM_FACTOR 250
#define INV_GYR_CMPF_FACTOR   (1.0f / (GYR_CMPF_FACTOR  + 1.0f))
#define INV_GYR_CMPFM_FACTOR  (1.0f / (GYR_CMPFM_FACTOR + 1.0f))


PUBLIC void InitIMU(void);
PUBLIC void ComputeIMU(void);
PRIVATE void GetEstimatedAttitude(imu_t* imu);
#endif
