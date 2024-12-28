#include "stm32f10x.h"                  // Device header
#include "delay.h"

/**
  * 蜂鸣器实验
  * 引脚：
  *      PB12
  * 现象：
  *      蜂鸣器开始鸣叫
  */
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); 						 
				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	while (1)
    {
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_RESET);// 0000 0000 0000 0000
		Delay_ms(500);	
		GPIO_WriteBit(GPIOB,GPIO_Pin_12,Bit_SET);// 0000 0000 0000 0001
		Delay_ms(300);	
    }
}
