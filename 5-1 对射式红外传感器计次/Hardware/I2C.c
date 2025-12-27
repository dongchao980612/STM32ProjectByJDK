#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "i2c.h"


extern I2CCfg_t g_i2cCfg;


void My_I2C_W_SCL(uint8_t  status)
{
    GPIO_WriteBit(g_i2cCfg.sclPort, g_i2cCfg.sclPin, (BitAction)status);
}


void My_I2C_W_SDA(uint8_t  status)
{
    GPIO_WriteBit(g_i2cCfg.sdaPort, g_i2cCfg.sdaPin, (BitAction)status);
}

uint8_t My_I2C_R_SDA(void)
{
    return GPIO_ReadInputDataBit(g_i2cCfg.sdaPort, g_i2cCfg.sdaPin);
}


void My_I2C_Delay(void)
{
    uint8_t i;

    for (i = 0; i < 10; i++);
}


/**
  * @brief  I2C初始化
  * @param  无
  * @retval 无
  */
void My_I2C_Init(void)
{
    RCC_APB2PeriphClockCmd(g_i2cCfg.sdaClock, ENABLE);
    RCC_APB2PeriphClockCmd(g_i2cCfg.sclClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = g_i2cCfg.sdaPin;
    GPIO_Init(g_i2cCfg.sdaPort, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = g_i2cCfg.sclPin;
    GPIO_Init(g_i2cCfg.sclPort, &GPIO_InitStructure);

    My_I2C_W_SCL(1);
    My_I2C_W_SDA(1);

}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void My_I2C_Start(void)
{
    My_I2C_W_SDA(1);
    My_I2C_W_SCL(1);
    My_I2C_Delay();

    My_I2C_W_SDA(0);
    My_I2C_Delay();

    My_I2C_W_SCL(0);
    My_I2C_Delay();
}

/**
  * @brief  I2C停止
  * @param  无
  * @retval 无
  */
void My_I2C_Stop(void)
{
    My_I2C_W_SDA(0);
    My_I2C_W_SCL(1);
    My_I2C_Delay();

    My_I2C_W_SDA(1);
}

/**
  * @brief  I2C发送一个字节
  * @param  Byte 要发送的一个字节
  * @retval 无
  */
void My_I2C_SendByte(uint8_t Byte)
{
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
        My_I2C_W_SDA(((Byte << i) & 0x80) ? 1 : 0);
        My_I2C_Delay();
        My_I2C_W_SCL(1);
        My_I2C_Delay();
        My_I2C_W_SCL(0);
        My_I2C_Delay();
    }
}

/**
  * @brief  I2C接受一个字节
  * @param  无
  * @retval 从从机读取到的8位数据
  */
uint8_t  My_I2C_ReceiveByte(void)   //主机读数据，从机写数据
{
    uint8_t i;
    uint8_t Byte = 0x00;
    My_I2C_W_SDA(1);  //主机释放SDA总线，由从机控制电平

    for(i = 0; i < 8; i++)
    {
        My_I2C_Delay();          // 修复1：SCL拉高前延时，准备时序
        My_I2C_W_SCL(1);         // SCL拉高，从机输出当前位到SDA
        My_I2C_Delay();          // 修复2：等待SDA电平稳定后再读取

        // 修复3：用左移拼接位，逻辑更规范、更稳定
        Byte <<= 1;              // 先把已读取的位左移1位，腾出最低位

        if(My_I2C_R_SDA() == 1)  // 读取当前SDA电平
        {
            Byte |= 0x01;        // 最低位置1（等价于加1）
        }

        // 若SDA为0，Byte最低位保持0，无需操作

        My_I2C_W_SCL(0);         // SCL拉低，从机准备下一位数据
        My_I2C_Delay();          // 修复4：拉低后延时，满足IIC时序
    }

    return Byte;
}

/**
  * @brief  I2C接收应答位
  * @param  无
  * @retval Ack 应答位
  */
uint8_t My_I2C_ReceiveACK(void)
{
    uint8_t Ack = 1;
    My_I2C_W_SDA(1); // 释放SDA总线，由OLED控制
    My_I2C_Delay();
    My_I2C_W_SCL(1); // 拉高SCL，读取应答电平
    My_I2C_Delay();


    if (My_I2C_R_SDA() == I2C_ACK)
    {
        Ack = 0;
    }

    My_I2C_W_SCL(0); // 拉低SCL，结束应答采样
    My_I2C_Delay();
    return Ack;
}

void My_I2C_SendACK(uint8_t ACKBit)
{
    My_I2C_W_SDA(ACKBit);
    My_I2C_W_SCL(1);
    My_I2C_W_SCL(0);
}
