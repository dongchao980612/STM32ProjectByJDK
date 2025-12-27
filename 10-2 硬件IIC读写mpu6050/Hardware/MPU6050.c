#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "iic.h"

MPU6050Cfg_t g_mpu6050Cfg =
{
    I2C1,
};


void  MPU6050_Init()
{
    I2C_Common_Init(g_mpu6050Cfg.iic);

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


// 封装超时等待I2C事件的函数
uint8_t I2C_WaitEvent_Timeout(I2C_TypeDef* I2Cx, uint32_t Event, uint32_t timeout)
{
    while(I2C_CheckEvent(I2Cx, Event) != SUCCESS)  // 使用参数I2Cx
    {
        timeout--;
        if(timeout == 0)
        {
            I2C_Cmd(I2Cx, DISABLE);  // 这里也要改
            I2C_Cmd(I2Cx, ENABLE);
            return 1;
        }
    }
    return 0;
}

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data)
{
    I2C_GenerateSTART(g_mpu6050Cfg.iic, ENABLE); //生成起始条件
    I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_MODE_SELECT,10000);// EV5

    I2C_Send7bitAddress(g_mpu6050Cfg.iic, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED,10000);  // EV6

    I2C_SendData(g_mpu6050Cfg.iic, RegAddress);
    I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTING,10000) ;// EV8

    I2C_SendData(g_mpu6050Cfg.iic, Data);
    I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTING,10000); // EV8

    I2C_GenerateSTOP(g_mpu6050Cfg.iic, ENABLE);
}


// 修复后的MPU6050读寄存器
uint8_t MPU6050_ReadReg(uint8_t RegAddress)
{
    uint8_t Data = 0xFF;
   

    // 1. 起始条件
    I2C_GenerateSTART(I2C1, ENABLE);
    if(I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_MODE_SELECT, 10000)) 
			return 0xFF;

    // 2. 发送从机地址（写）
    I2C_Send7bitAddress(I2C1, MPU6050_ADDRESS, I2C_Direction_Transmitter);
    if(I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, 10000)) 
			return 0x11;

    // 3. 发送寄存器地址
    I2C_SendData(I2C1, RegAddress);
    if(I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTED, 10000)) 
			return 0x00;

    // 4. 重复起始条件
    I2C_GenerateSTART(g_mpu6050Cfg.iic, ENABLE);
    if(I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_MODE_SELECT, 10000)) 
			return 0x00;

    // 5. 发送从机地址（读）
    I2C_Send7bitAddress(g_mpu6050Cfg.iic, MPU6050_ADDRESS, I2C_Direction_Receiver);
    if(I2C_WaitEvent_Timeout(g_mpu6050Cfg.iic, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, 10000)) 
			return 0x00;

    // 6. 关闭ACK（单字节接收）
    I2C_AcknowledgeConfig(g_mpu6050Cfg.iic, DISABLE);

    // 7. 读取数据
    Data = I2C_ReceiveData(g_mpu6050Cfg.iic);

    // 8. 发送停止条件
    I2C_GenerateSTOP(g_mpu6050Cfg.iic, ENABLE);

    // 9. 恢复ACK使能
    I2C_AcknowledgeConfig(g_mpu6050Cfg.iic, ENABLE);

    return Data;
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
