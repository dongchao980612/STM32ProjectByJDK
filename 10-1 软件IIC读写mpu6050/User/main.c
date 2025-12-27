#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "mpu6050.h"
#include "i2c.h"
#include "mpu6050_reg.h"
#include "delay.h"

I2CCfg_t g_i2cCfg =
{
    /* SCL Pin*/
    RCC_APB2Periph_GPIOD,
    GPIO_Pin_3,
    GPIOD,

    /* SDA Pin */
    RCC_APB2Periph_GPIOG,
    GPIO_Pin_13,
    GPIOG,
};


int16_t AX, AY, AZ, GX, GY, GZ, TMP;
float ax, ay, az, gx, gy, gz, temp;
uint8_t id;  // 存储传感器ID

int main(void)
{
    /*模块初始化*/
    OLED_Init();		//OLED初始化
    MPU6050_Init();//mpu6050初始化

    #if 0
    // My_I2C_Init();          // 初始化I2C总线

    My_I2C_Start();         // 发送I2C起始信号

    My_I2C_SendByte(MPU6050_ADDRESS);  // 发送MPU6050的设备地址（写操作）
    uint8_t Ack = 0;

    Ack = My_I2C_ReceiveACK();            // 接收从机的应答信号

    // 判断是否收到有效应答
    if(Ack == I2C_ACK)  // 补全判断条件：收到应答表示通信成功
    {
        OLED_ShowString(1, 1, "SUCCESS!");  // 1行1列显示成功
    }
    else  // 补全else分支：未收到应答表示通信失败
    {
        OLED_ShowString(1, 1, "ERROR!");    // 1行1列显示失败
    }

    My_I2C_Stop();  // 发送I2C停止信号（避免总线挂死）


    #endif

    #if 0

    OLED_ShowHexNum(1, 4, MPU6050_GetID(), 2);

    #endif

    #if 0

    MPU6050_WriteReg(0X6B, 0X00);
    MPU6050_WriteReg(0X19, 0X66);
    OLED_ShowHexNum(1, 4, MPU6050_ReadReg(0X19), 2);
    #endif


    // 1. 读取传感器ID并显示（第一行）
    id = MPU6050_GetID();
    OLED_ShowString(1, 1, "ID: 0x");          // 1行1列：ID标识
    OLED_ShowHexNum(1, 6, id, 2);      // 1行6列：显示2位16进制ID（0x68/0x70）

    // 识别传感器型号并显示
    if(id == 0x68 || id == 0x69)
    {
        OLED_ShowString(1, 9, "6050");        // 1行9列：显示型号
    }
    else if(id == 0x70 || id == 0x71)
    {
        OLED_ShowString(1, 9, "6500");        // 1行9列：显示型号
    }
    else
    {
        OLED_ShowString(1, 9, "Unknown");     // 未知型号
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
        MPU6050_GetData(&ax, &ay, &az, &gx, &gx, &gx, &temp);


        // 4. 显示温度（2行：整数+小数+单位）
        OLED_ShowSignedNum(2, 6, (int)temp, 2);             // 温度整数部分（如25）
        // OLED_ShowString(2,8,".");
        OLED_ShowNum(2, 10, (uint8_t)((temp - (int)temp) * 10), 1);  // 小数1位（如3）

        // 5. 显示加速度（保留1位小数，单位g）
        //OLED_ShowNum(3, 6, (int)ax, 2);               // AccX整数部分
        //OLED_ShowNum(3, 9, (uint8_t)((ax - (int)ax) * 10), 1); // AccX小数1位

        //OLED_ShowNum(4, 6, (int)ay, 2);               // AccY整数部分
        //OLED_ShowNum(4, 9, (uint8_t)((ay - (int)ay) * 10), 1); // AccY小数1位

        Delay_us(5);

    }
}
