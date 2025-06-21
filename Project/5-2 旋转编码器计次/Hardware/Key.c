#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "key.h"


static KeyCfg_t g_keyCfg = {
	/* Key */
	GPIOC,
	RCC_APB2Periph_GPIOC,
	GPIO_Pin_14,
};



void Key_Init(void){
	RCC_APB2PeriphClockCmd(g_keyCfg.keyClock,ENABLE); 

	GPIO_InitTypeDef GPIO_InitStructure;	 
				 
	GPIO_InitStructure.GPIO_Pin =  g_keyCfg.keyPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(g_keyCfg.keyPort, &GPIO_InitStructure);
	
}

uint8_t Key_GetNum(void){
	uint8_t keyNum = 0;
	
	if(GPIO_ReadInputDataBit(g_keyCfg.keyPort,g_keyCfg.keyPin)==0) //按下
	{
		Delay_ms(20);
		while(GPIO_ReadInputDataBit(g_keyCfg.keyPort,g_keyCfg.keyPin)==0){
			Delay_ms(20);
			keyNum = 1;
			
		}
	}
	return keyNum;
}
