#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "delay.h"

int16_t AX = 0, AY = 0, AZ = 0, GX = 0, GY = 0, GZ = 0, TMP = 0;
float ax = 0.0f, ay = 0.0f, az = 0.0f, gx = 0.0f, gy = 0.0f, gz = 0.0f, temp = 0.0f;
uint8_t sensor_id = 0; // 存储传感器ID

int main(void)
{
    /*模块初始化*/
    OLED_Init();		//OLED初始化
    // OLED_ShowString(1, 1, "OLED Init OK");

    MPU6050_Init();//mpu6050初始化

    // 1. 读取传感器ID并显示（第一行）
    sensor_id = MPU6050_GetID();
    OLED_ShowString(1, 1, "ID: 0x");          // 1行1列：ID标识
    OLED_ShowHexNum(1, 6, sensor_id, 2);      // 1行6列：显示2位16进制ID（0x68/0x70）

    // 识别传感器型号并显示
    if(sensor_id == 0x68 || sensor_id == 0x69)
    {
        OLED_ShowString(1, 9, "6050");        // 1行9列：显示型号
    }
    else if(sensor_id == 0x70 || sensor_id == 0x71)
    {
        OLED_ShowString(1, 9, "6500");        // 1行9列：显示型号
    }
    else
    {
        OLED_ShowString(1, 9, "Unknown");     // 未知型号
    }


    while(1)
    {
        // 3. 读取传感器数据
        MPU6050_GetData_Raw(&AX, &AY, &AZ, &GX, &GY, &GZ, &TMP);
        OLED_ShowSignedNum(2, 1, AX, 5);					//OLED显示数据
        OLED_ShowSignedNum(3, 1, AY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
    }
}


