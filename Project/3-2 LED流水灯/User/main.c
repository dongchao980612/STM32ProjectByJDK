#include "stm32f10x.h"                  // Device header
#include "delay.h"

typedef struct{
	/* Led */
	GPIO_TypeDef*       ledPort;
	uint32_t            ledClock;
	uint16_t            ledPin1;
	uint16_t            ledPin2;
	uint16_t            ledPin3;
	uint32_t			ledDelay;
} LedCfg_t;

static LedCfg_t g_ledCfg = {
	/* Led */
	GPIOC,
	RCC_APB2Periph_GPIOC,
	GPIO_Pin_11,
	GPIO_Pin_12,
	GPIO_Pin_13,
	1000
};

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(g_ledCfg.ledClock,ENABLE); 						 
				 
	GPIO_InitStructure.GPIO_Pin =  g_ledCfg.ledPin1|g_ledCfg.ledPin2|g_ledCfg.ledPin3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(g_ledCfg.ledPort, &GPIO_InitStructure);

	while (1)
    {
		GPIO_Write(g_ledCfg.ledPort,~0X1000); // 0000 0000 0000 0000
		Delay_ms(g_ledCfg.ledDelay);	
		GPIO_Write(g_ledCfg.ledPort,~0X2000); // 0000 0000 0000 0000
		Delay_ms(g_ledCfg.ledDelay);	
    }
}
