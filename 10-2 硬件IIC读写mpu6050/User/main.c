#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "delay.h"



int16_t AX, AY, AZ, GX, GY, GZ, TMP;
float ax, ay, az, gx, gy, gz, temp;
uint8_t sensor_id;  // 存储传感器ID

int main(void)
{
    /*模块初始化*/
    OLED_Init();		//OLED初始化
    MPU6050_Init();//mpu6050初始化

    // 1. 读取传感器ID并显示（第一行）
    sensor_id = MPU6050_GetID();
    // OLED_ShowString(1, 1, "ID: 0x");          // 1行1列：ID标识
    // OLED_ShowHexNum(1, 7, sensor_id, 2);      // 1行6列：显示2位16进制ID（0x68/0x70）

    // 识别传感器型号并显示
    if(sensor_id == 0x68 || sensor_id == 0x69)
    {
        OLED_ShowString(1, 10, "6050");        // 1行9列：显示型号
    }
    else if(sensor_id == 0x70 || sensor_id == 0x71)
    {
        OLED_ShowString(1, 10, "6500");        // 1行9列：显示型号
    }
    else
    {
        OLED_ShowHexNum(1, 1, sensor_id,3);     // 未知型号
    }

    // 2. 固定显示表头（第二、三、四行）
    OLED_ShowString(2, 1, "Temp:   . C");    // 2行：温度显示区（预留整数+小数+单位）
    // OLED_ShowString(3, 1, "AccX:     g");    // 3行：X轴加速度
    // OLED_ShowString(4, 1, "AccY:     g");    // 4行：Y轴加速度
    // OLED_ShowString(5, 1, "AccZ:   .  g");

    while(1)
    {
        // 3. 读取传感器数据
        // 方式1：用GetData直接读取物理量
        // MPU6050_GetData(&ax, &ay, &az, &gx, &gx, &gx, &temp);
        temp = MPU6050_GetTemp();

        // 4. 显示温度（2行：整数+小数+单位）
        OLED_ShowSignedNum(2, 6, (int)temp, 2);             // 温度整数部分（如25）
        // OLED_ShowString(2,8,".");
        OLED_ShowNum(2, 10, (uint8_t)((temp - (int)temp) * 10), 1);  // 小数1位（如3）

        // 5. 显示加速度（保留1位小数，单位g）
        // OLED_ShowNum(3, 6, (int)ax, 2);               // AccX整数部分
        // OLED_ShowNum(3, 9, (uint8_t)((ax - (int)ax) * 10), 1); // AccX小数1位

        // OLED_ShowNum(4, 6, (int)ay, 2);               // AccY整数部分
        // OLED_ShowNum(4, 9, (uint8_t)((ay - (int)ay) * 10), 1); // AccY小数1位

    }
}


