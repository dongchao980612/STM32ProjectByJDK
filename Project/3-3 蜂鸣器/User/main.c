#include "stm32f10x.h"                  // Device header
#include "delay.h"

/**
  * 蜂鸣器实验
  * 引脚：
  *      PC13
  * 现象：
  *      蜂鸣器开始鸣叫
  */
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 						 
				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	while (1)
    {
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET); 
		Delay_ms(100);	
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET); 
		Delay_ms(100);	
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET); 
		Delay_ms(100);	
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET); 
		Delay_ms(700);
    }
}
