#include "stm32f10x.h"                  // Device header


void LightSensor_Init(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE); 

	GPIO_InitTypeDef GPIO_InitStructure;	 
				 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_15);
	
}

uint8_t LightSensor_GetNum(void){
	
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15);
	
}