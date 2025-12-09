#include "stm32f10x.h"                  // Device header
#include "usart.h"
#include "delay.h"
#include <stdio.h>

uint8_t RxData;			//定义用于接收串口数据的变量


int main(void)
{
    My_USART_Init();

    while (1)
    {
//			if (USART_GetRxFlag() == 1)	{
//			RxData = USART_GetRxData();		//获取串口接收的数据
//			My_USART_SendByte(RxData);			//串口将收到的数据回传回去，用于测试
//			printf("\r\nRxData=%d", RxData);	
//				
//			}
    }
}
