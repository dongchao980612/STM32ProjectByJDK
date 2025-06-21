#include "stm32f10x.h"                  // Device header
#include "delay.h"

typedef struct{
	/* Buzzer */
	GPIO_TypeDef*       buzzerPort;
	uint32_t            buzzerClock;
	uint16_t            buzzerPin;
	uint32_t						buzzerDelay100;
	uint32_t						buzzerDelay700;
} BuzzerCfg_t;

static BuzzerCfg_t g_buzzerCfg = {
	/* Buzzer */
	GPIOC,
	RCC_APB2Periph_GPIOC,
	GPIO_Pin_13,
	100,700
};


int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(g_buzzerCfg.buzzerClock,ENABLE); 						 
				 
	GPIO_InitStructure.GPIO_Pin =  g_buzzerCfg.buzzerPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(g_buzzerCfg.buzzerPort, &GPIO_InitStructure);

	while (1)
    {
		GPIO_WriteBit(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin,Bit_RESET); 
		Delay_ms(g_buzzerCfg.buzzerDelay100);	
		GPIO_WriteBit(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin,Bit_SET); 
		Delay_ms(g_buzzerCfg.buzzerDelay100);	
		GPIO_WriteBit(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin,Bit_RESET); 
		Delay_ms(g_buzzerCfg.buzzerDelay100);	
		GPIO_WriteBit(g_buzzerCfg.buzzerPort,g_buzzerCfg.buzzerPin,Bit_SET); 
		Delay_ms(g_buzzerCfg.buzzerDelay700);
    }
}
