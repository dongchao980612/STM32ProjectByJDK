#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "iic.h"
#include "delay.h"

MPU6050Cfg_t g_mpu6050Cfg =
{
    I2C1,
};


void  MPU6050_Init()
{
    I2C_Common_Init(g_mpu6050Cfg.iic);

    //不复位、解除睡眠、不开启循环模式、温度传感器失能、选择陀螺仪x轴时钟
    MPU6050_WriteReg(MPU6050_PWR_MGMT_1, 0x01);
    //没有开启循环模式
    MPU6050_WriteReg(MPU6050_PWR_MGMT_2, 0x00);
    //采样率10分频
    MPU6050_WriteReg(MPU6050_SMPLRT_DIV, 0x09);
    //不使用外部同步、DLPF设置等级6
    MPU6050_WriteReg(MPU6050_CONFIG, 0x06);
    //陀螺仪：自测失能、满量程±500°/s-000_01_000
    MPU6050_WriteReg(MPU6050_GYRO_CONFIG, 0x08);
    //加速度计：自测失能、满量程±2g、失能运动检测-000_00_000
    MPU6050_WriteReg(MPU6050_ACCEL_CONFIG, 0x00);

}

uint8_t MPU6050_GetID(void)
{
    return MPU6050_ReadReg(MPU6050_WHO_AM_I);
}




void MPU6050_WriteReg(uint8_t RegAddr, uint8_t wData)
{
    // 1.发送起始位
    I2C_GenerateSTART(g_mpu6050Cfg.iic, ENABLE);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件发生

    // 2.发送从机地址
    I2C_Send7bitAddress(g_mpu6050Cfg.iic, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//等待EV6事件发生

    //3.发送寄存器地址
    I2C_SendData(g_mpu6050Cfg.iic, RegAddr);//这一步无需等到EV8_1事件发生
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTING);//等待EV8事件发生

    //4.发送数据
    I2C_SendData(g_mpu6050Cfg.iic, wData);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//等待EV8_2事件发生
	
    //5.发送停止位
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, ENABLE);

}

//MPU6050指定地址读寄存器
uint8_t MPU6050_ReadReg(uint8_t RegAddr)
{
    uint8_t rData = 0x00;

    // 1.发送起始位
    I2C_GenerateSTART(g_mpu6050Cfg.iic, ENABLE);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件发生

    // 2.发送从机地址
    I2C_Send7bitAddress(g_mpu6050Cfg.iic, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED);//等待EV6事件发生

    // 3.发送寄存器地址
    I2C_SendData(g_mpu6050Cfg.iic, RegAddr);//这一步无需等到EV8_1事件发生
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTED);//等待EV8_2事件发生


    // 4.再次发送起始位
    I2C_GenerateSTART(g_mpu6050Cfg.iic, ENABLE);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_MODE_SELECT);//等待EV5事件发生

    // 5.发送从机地址
    I2C_Send7bitAddress(g_mpu6050Cfg.iic, MPU6050_ADDRESS, I2C_Direction_Receiver);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED);//等待EV6事件发生

    // 6.按照手册，需要首先设置ACK=0、并且产生停止条件
    I2C_AcknowledgeConfig(g_mpu6050Cfg.iic, DISABLE);
    I2C_GenerateSTOP(I2C2, ENABLE);
    I2C_TimeOutEvent(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_RECEIVED);//等待EV7事件发生

    // 7.接收数据
    rData = I2C_ReceiveData(g_mpu6050Cfg.iic);
    I2C_AcknowledgeConfig(g_mpu6050Cfg.iic, ENABLE);//将ACK改回默认的使能配置，方便指定地址收多个字节

    return rData;
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
    *Temp = (float)Temp_Raw / 340.0f + 36.53f;

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


    float temp = (float)Temp_Raw / 340.0f + 36.53f;
    return temp;
}
