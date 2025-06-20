#include "stm32f10x.h"                  // Device header

typedef struct{
	/* Buzzer */
	GPIO_TypeDef*       buzzerPort;
	uint32_t            buzzerClock;
	uint16_t            buzzerPin;
} buzzerCfg_t;


static buzzerCfg_t g_buzzerCfg = {
	/* Buzzer */
	GPIOC,
	RCC_APB2Periph_GPIOC,
	GPIO_Pin_11,
};

void Buzzer_Init(void){
	RCC_APB2PeriphClockCmd(g_buzzerCfg.buzzerClock,ENABLE); 

	GPIO_InitTypeDef GPIO_InitStructure;	 
				 
	GPIO_InitStructure.GPIO_Pin =  g_buzzerCfg.buzzerPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(g_buzzerCfg.buzzerPort, &GPIO_InitStructure);
	
	GPIO_SetBits(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin);

}

void Buffer_Trun(void)
{
	if(GPIO_ReadOutputDataBit(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin)==0)
	{
		GPIO_SetBits(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin);
	}
	else{
		GPIO_ResetBits(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin);
	}
}

void Buzzer_ON(void){
	 GPIO_ResetBits(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin);
}

void Buzzer_OFF(void){
	 GPIO_SetBits(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin);
}
