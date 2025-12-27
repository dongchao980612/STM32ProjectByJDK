#ifndef __MPU6050_H
#define __MPU6050_H

typedef struct
{
    I2C_TypeDef* iic;          // I2C外设（如I2C1）
} MPU6050Cfg_t;

// MPU设备地址
#define MPU6050_ADDRESS      				0x68 // (0x68<<1)     


// 加速度计配置：0x18 → ±16g
#define MPU6050_ACC_SCALE   (1.0f / 2048.0f)  // g per LSB


// 陀螺仪配置：0x18 → ±2000°/s
#define MPU6050_GYRO_SCALE  (1.0f / 16.4f)    // °/s per LSB



void MPU6050_Init(void);

void  I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t Event);

uint8_t MPU6050_ReadReg(uint8_t Reg_Address);
void MPU6050_WriteReg(uint8_t Reg_Address, uint8_t Data);


uint8_t MPU6050_GetID(void);


void MPU6050_GetData_Raw(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ, int16_t *Temp);
void MPU6050_GetData(float *AccX, float *AccY, float *AccZ, float *GyroX, float *GyroY, float *GyroZ, float *Temp);


float MPU6050_GetAx(void);
float MPU6050_GetAy(void);
float MPU6050_GetAz(void);

float MPU6050_GetGx(void);
float MPU6050_GetGy(void);
float MPU6050_GetGz(void);

float MPU6050_GetTemp(void);

#endif
