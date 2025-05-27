#include "stm32f10x.h"                  // Device header
#include "delay.h"
/**
  * LED流水灯实验
  * 引脚：
  *      PB13-PB14
  * 现象：
  *      LED被点亮
  */
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 						 
				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	while (1)
    {
		GPIO_Write(GPIOC,~0X1000); // 0000 0000 0000 0000
		Delay_ms(500);	
		GPIO_Write(GPIOC,~0X2000); // 0000 0000 0000 0000
		Delay_ms(500);	
    }
}
