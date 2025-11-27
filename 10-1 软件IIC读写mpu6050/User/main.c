#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "myi2c.h"

int main(void)
{
    OLED_Init();		//OLED初始化

    MyI2C_Init();

    MyI2C_Start();

    MyI2C_SendByte(0xD0); //1101 0000

    uint8_t Ack  = MyI2C_RecvACK();


    MyI2C_Stop();

    OLED_ShowNum(1, 1, Ack, 3);


    while (1)
    {

    }
}
