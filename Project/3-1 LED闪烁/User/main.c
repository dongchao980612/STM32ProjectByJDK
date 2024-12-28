#include "stm32f10x.h"                  // Device header
#include "delay.h"

/**
  * 点亮LED实验
  * 引脚：
  *      PC13
  * 现象：
  *      LED被点亮
  */
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 						 
				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// GPIO_SetBits(GPIOC,GPIO_Pin_13); // high disable led
	// GPIO_ResetBits(GPIOC,GPIO_Pin_13); // low ensable led
	// GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);// Bit_RESET->1 Bit_SET->0
	// GPIO_Write();
	
	while (1)
    {
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
		Delay_ms(500);
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
		Delay_ms(500);
	
		// GPIO_SetBits(GPIOC,GPIO_Pin_13); // high enable led
		// Delay_s(1);
		// GPIO_ResetBits(GPIOC,GPIO_Pin_13); // low disable led
		// Delay_s(1);
    }
}
