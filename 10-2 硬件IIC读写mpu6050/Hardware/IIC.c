#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "iic.h"
#include "delay.h"


void I2C_Common_Init(I2C_TypeDef* I2Cx)
{
    // 1. 开启时钟（PB6/PB7属于GPIOB，I2C1属于APB1）
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // 2. 配置GPIO为复用开漏输出
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;  // 复用开漏（必须）
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    // 3. 配置I2C参数
    I2C_InitTypeDef I2C_InitStructure;
    I2C_InitStructure.I2C_ClockSpeed = 100000;        // 100kHz
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;// 50%占空比
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;         // 主机无地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;       // 使能ACK
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
    I2C_Init(I2Cx, &I2C_InitStructure);

    // 4. 清除错误标志
    I2C_ClearFlag(I2Cx, I2C_FLAG_AF | I2C_FLAG_ARLO | I2C_FLAG_BERR);

    // 5. 使能I2C
    I2C_Cmd(I2Cx, ENABLE);
}

void I2C_TimeOutEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT)
{
    uint32_t Timeout;
    Timeout = 10000;

    while(I2C_CheckEvent(I2Cx, I2C_EVENT) == ERROR)
    {
        Timeout--;

        if(Timeout == 0)
        {
            break;
        }
    }
}

