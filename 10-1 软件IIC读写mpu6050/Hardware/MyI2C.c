#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define SCL_PORT GPIOB
#define SCL_PIN GPIO_Pin_3
#define SDA_PORT GPIOB
#define SDA_PIN GPIO_Pin_4


//利用函数将写数据为封装起来
void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(SCL_PORT, GPIO_Pin_3, (BitAction)BitValue);
    Delay_us(10);
}
void  MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(SDA_PORT, SDA_PIN, (BitAction)BitValue);
    Delay_us(10);
}
uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(SDA_PORT, SDA_PIN);
    Delay_us(10);
    return BitValue;
}

void MyI2C_Init(void)
{
    //将SCL和SDA配置位开漏输出模式
    //将SCL和SDA均置为高电平
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;  //开漏输出模式
    GPIO_InitStruct.GPIO_Pin = SCL_PIN | SDA_PIN;
    GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_50MHz;
    GPIO_Init(SDA_PORT, &GPIO_InitStruct);
    GPIO_SetBits(SDA_PORT, SCL_PIN | SDA_PIN);

}
//起始条件
void MyI2C_Start(void)
{
    MyI2C_W_SCL(1); //将SCL和SDA都释放
    MyI2C_W_SDA(1);
    MyI2C_W_SDA(0); //拉低SDA
    MyI2C_W_SCL(0); //拉低SCL
}
//终止条件

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0); //先拉低SDA
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}


//发送一个字节
void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;	//1000 0000

    for(i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(Byte & (0x80 >> i) );//高位先行，首先趁着SCL低电平，将byte的最高位放到SDA上。
        //结果不是 0x00 就是0x80
        MyI2C_W_SCL(1); //SCL为高电平 //从机读取数据
        MyI2C_W_SCL(0);//SCL为低电平  //主机下一次写数据
    }
}
//接收一个字节
uint8_t  MyI2C_RecByte(void)   //主机读数据，从机写数据
{
    uint8_t i;
    uint8_t Byte = 0x00;
    MyI2C_W_SDA(1);  //主机将SDA释放 ，为高电平，切换为输入模式

    for(i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);  //SCL为高电平开始读SDA

        if(MyI2C_R_SDA() == 1)
        {
            Byte  |= 0x80 >> i;
        }

        //按位或  ，byte =1 ，则 byte最高位置为1
        //如果if不成立则写入0
        MyI2C_W_SCL(0);
    }

    return Byte;
}
//发送应答位
void MyI2C_SendACK(uint8_t ACKBit)
{
    MyI2C_W_SDA(ACKBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}
//接收应答位
uint8_t  MyI2C_RecvACK(void)   //主机读数据，从机写数据
{
    uint8_t ACKBit;
    MyI2C_W_SDA(1);   //即使主机释放SDA为高电平，而从机写数据到SDA
    MyI2C_W_SCL(1);  //SCL为高电平开始读SDA
    ACKBit = MyI2C_R_SDA();    //读取的
    MyI2C_W_SCL(0);
    return ACKBit;
}

