#include "stm32f10x.h"                  // Device header

typedef struct{
	/* LED */
	GPIO_TypeDef*       ledPort;
	uint32_t            ledClock;
	uint16_t            ledPin;
} LedCfg_t;


static LedCfg_t g_ledCfg = {
	/* Led */
	GPIOC,
	RCC_APB2Periph_GPIOC,
	GPIO_Pin_10,
};



void LED_Init(void){
	RCC_APB2PeriphClockCmd(g_ledCfg.ledClock,ENABLE); 

	GPIO_InitTypeDef GPIO_InitStructure;	 
				 
	GPIO_InitStructure.GPIO_Pin =  g_ledCfg.ledPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(g_ledCfg.ledPort,GPIO_Pin_13);
 }

void LED_Trun()
{
	if(GPIO_ReadOutputDataBit(g_ledCfg.ledPort,g_ledCfg.ledPin)==0)
	{
		GPIO_SetBits(g_ledCfg.ledPort,g_ledCfg.ledPin);
	}
	else{
		GPIO_ResetBits(g_ledCfg.ledPort,g_ledCfg.ledPin);
	}
}

void LED_ON(void){
	 GPIO_ResetBits(g_ledCfg.ledPort,g_ledCfg.ledPin);
}

void LED_OFF(void){
	 GPIO_SetBits(g_ledCfg.ledPort,g_ledCfg.ledPin);
}
