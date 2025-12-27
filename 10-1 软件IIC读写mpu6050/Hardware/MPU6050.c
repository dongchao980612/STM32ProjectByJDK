#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "i2c.h"
#include "mpu6050.h"
#include "mpu6050_reg.h"


void  MPU6050_Init()
{
    My_I2C_Init();

    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);		// 配置PWR_MGMT_1寄存器，解除睡眠，选择X轴陀螺仪时钟
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00); 	// 配置PWR_MGMT_2寄存器
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);		// 采样率分频，决定了数据输出的快慢。10分频
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);				// 数字低通滤波器给110
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x18);	// 陀螺仪配置，选择最大量程11
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x18);	// 加速度计配置，选择最大量程11

}

uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    My_I2C_Start();
    My_I2C_SendByte(MPU6050_ADDRESS);
    My_I2C_ReceiveACK();

    My_I2C_SendByte(RegAddress);
    My_I2C_ReceiveACK();

    My_I2C_SendByte(Data);
    My_I2C_ReceiveACK();

    My_I2C_Stop();

}

uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0xFF; // 初始化失败标记

    My_I2C_Start();
    My_I2C_Delay();

    // 步骤1：发送MPU6050写地址（0xD0）
    My_I2C_SendByte(MPU6050_ADDRESS);

    if (My_I2C_ReceiveACK() != I2C_ACK) // 检查从机ACK
    {
        My_I2C_Stop();
        return Data; // 无ACK，返回0xFF
    }

    // 步骤2：发送要读取的寄存器地址
    My_I2C_SendByte(RegAddress);

    if (My_I2C_ReceiveACK() != I2C_ACK)
    {
        My_I2C_Stop();
        return Data; // 无ACK，返回0xFF
    }

    // 步骤3：重复起始信号，切换为读模式
    My_I2C_Start();
    My_I2C_SendByte(MPU6050_ADDRESS | 0x01); // 读地址（0xD1）

    if (My_I2C_ReceiveACK() != I2C_ACK)
    {
        My_I2C_Stop();
        return Data; // 无ACK，返回0xFF
    }

    // 步骤4：读取寄存器数据（单字节，发送NACK）
    Data = My_I2C_ReceiveByte(); // 覆盖初始的0xFF
    My_I2C_SendACK(I2C_NACK);    // 单字节读取，发送非应答

    My_I2C_Stop();
    My_I2C_Delay();

    return Data; // 成功返回有效数据，失败返回0xFF
}

/**
 * @brief  一次性读取所有原始数据（加速度、陀螺仪、温度）
 * @param  AccX/AccY/AccZ: 加速度计原始值（int16_t）
 * @param  GyroX/GyroY/GyroZ: 陀螺仪原始值（int16_t）
 * @param  tmp: 温度原始值（int16_t）
 */
void MPU6050_GetData_Raw(int16_t *AccX, int16_t *AccY, int16_t *AccZ, int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ, int16_t *tmp)
{
    uint8_t Data_H, Data_L;

    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//    加速度寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    *AccX = (Data_H << 8) | Data_L;//   加速度计X轴的16位数据

    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    *AccY = (Data_H << 8) | Data_L;

    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    *AccZ = (Data_H << 8) | Data_L;

    Data_H = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);//    陀螺仪寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    *GyroX = (Data_H << 8) | Data_L;//   陀螺仪X轴的16位数据

    Data_H = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    *GyroY = (Data_H << 8) | Data_L;

    Data_H = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    *GyroZ = (Data_H << 8) | Data_L;

    Data_H = MPU6050_ReadReg(MPU6050_TEMP_OUT_H);//    温度寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_TEMP_OUT_L);
    *tmp = (Data_H << 8) | Data_L;//   温度的16位数据

}

/**
 * @brief  一次性读取所有物理量（加速度g、陀螺仪°/s、温度℃）
 * @param  AccX/AccY/AccZ: 加速度计值（g）
 * @param  GyroX/GyroY/GyroZ: 陀螺仪值（°/s）
 * @param  Temp: 温度值（℃）
 */
void MPU6050_GetData(float *AccX, float *AccY, float *AccZ, float *GyroX, float *GyroY, float *GyroZ, float *Temp)
{
    // 先读取原始值（复用已有函数，避免重复代码）
    int16_t AccX_Raw, AccY_Raw, AccZ_Raw;
    int16_t GyroX_Raw, GyroY_Raw, GyroZ_Raw;
    int16_t Temp_Raw;
    MPU6050_GetData_Raw(&AccX_Raw, &AccY_Raw, &AccZ_Raw,
                        &GyroX_Raw, &GyroY_Raw, &GyroZ_Raw, &Temp_Raw);

    // 加速度计换算（g）
    *AccX = AccX_Raw * MPU6050_ACC_SCALE;
    *AccY = AccY_Raw * MPU6050_ACC_SCALE;
    *AccZ = AccZ_Raw * MPU6050_ACC_SCALE;

    // 陀螺仪换算（°/s）
    *GyroX = GyroX_Raw * MPU6050_GYRO_SCALE;
    *GyroY = GyroY_Raw * MPU6050_GYRO_SCALE;
    *GyroZ = GyroZ_Raw * MPU6050_GYRO_SCALE;

    // 温度换算（℃）
    *Temp = (float)Temp_Raw / 333.87f+21.0f;

}

/**
 * @brief  读取加速度计X轴物理值（单位：g）
 * @retval X轴加速度（g）
 */
float MPU6050_GetAx(void)
{
    uint8_t Data_H, Data_L;
    int16_t AccX_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_H);//    加速度寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_XOUT_L);
    AccX_Raw = (Data_H << 8) | Data_L;//   加速度计X轴的16位数据

    float AccX = AccX_Raw * MPU6050_ACC_SCALE;

    return AccX;

}

/**
 * @brief  读取加速度计Y轴物理值（单位：g）
 * @retval Y轴加速度（g）
 */
float MPU6050_GetAy(void)
{
    uint8_t Data_H, Data_L;
    int16_t AccY_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_H);//    加速度寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_YOUT_L);
    AccY_Raw = (Data_H << 8) | Data_L;//   加速度计X轴的16位数据

    float AccY = AccY_Raw * MPU6050_ACC_SCALE;

    return AccY;
}

/**
 * @brief  读取加速度计Z轴物理值（单位：g）
 * @retval Z轴加速度（g）
 */
float MPU6050_GetAz(void)
{
    uint8_t Data_H, Data_L;
    int16_t AccZ_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_H);//    加速度寄存器Y轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_ACCEL_ZOUT_L);
    AccZ_Raw = (Data_H << 8) | Data_L;//   加速度计X轴的16位数据

    float AccZ = AccZ_Raw * MPU6050_ACC_SCALE;
    return AccZ;
}

/**
 * @brief  读取陀螺仪X轴物理值（单位：°/s）
 * @retval X轴角速度（°/s）
 */
float MPU6050_GetGx(void)
{
    uint8_t Data_H, Data_L;
    int16_t GyroX_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_XOUT_H);//    陀螺仪寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_XOUT_L);
    GyroX_Raw = (Data_H << 8) | Data_L;//   陀螺仪X轴的16位数据

    float GyroX = GyroX_Raw * MPU6050_GYRO_SCALE;
    return GyroX;

}

/**
 * @brief  读取陀螺仪Y轴物理值（单位：°/s）
 * @retval Y轴角速度（°/s）
 */
float MPU6050_GetGy(void)
{
    uint8_t Data_H, Data_L;
    int16_t GyroY_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_YOUT_H);//    陀螺仪寄存器Y轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_YOUT_L);
    GyroY_Raw = (Data_H << 8) | Data_L;//   陀螺仪X轴的16位数据

    float GyroY = GyroY_Raw * MPU6050_GYRO_SCALE;
    return GyroY;
}

/**
 * @brief  读取陀螺仪Z轴物理值（单位：°/s）
 * @retval Z轴角速度（°/s）
 */
float MPU6050_GetGz(void)
{
    uint8_t Data_H, Data_L;
    int16_t GyroZ_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_H);//    陀螺仪寄存器Z轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_GYRO_ZOUT_L);
    GyroZ_Raw = (Data_H << 8) | Data_L;//   陀螺仪X轴的16位数据

    float GyroZ = GyroZ_Raw * MPU6050_GYRO_SCALE;
    return GyroZ;
}

/**
 * @brief  读取MPU6050温度（单位：℃）
 * @retval 温度值（℃）
 */
float MPU6050_GetTemp()
{
    uint8_t Data_H, Data_L;
    int16_t Temp_Raw;
    Data_H = MPU6050_ReadReg(MPU6050_TEMP_OUT_H);//    温度寄存器X轴高8位
    Data_L = MPU6050_ReadReg(MPU6050_TEMP_OUT_L);
    Temp_Raw = (Data_H << 8) | Data_L;//   温度的16位数据


    float temp = (float)Temp_Raw / 333.87f+21.0f;
    return temp;
}
