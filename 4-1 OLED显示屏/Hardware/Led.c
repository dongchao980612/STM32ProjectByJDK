#include "stm32f10x.h"                  // Device header
#include "led.h"

/* LED */
static LedCfg_t g_ledCfg =
{
    GPIOB,
    RCC_APB2Periph_GPIOB,
    GPIO_Pin_5
};

void led_init(void)
{
    RCC_APB2PeriphClockCmd(g_ledCfg.ledClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin =  g_ledCfg.ledPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(g_ledCfg.ledPort, &GPIO_InitStructure);
	
		GPIO_SetBits(g_ledCfg.ledPort,g_ledCfg.ledPin); 
}

void led_turn()
{
    if(GPIO_ReadOutputDataBit(g_ledCfg.ledPort, g_ledCfg.ledPin) == 0)
    {
        GPIO_SetBits(g_ledCfg.ledPort, g_ledCfg.ledPin);
    }
    else
    {
        GPIO_ResetBits(g_ledCfg.ledPort, g_ledCfg.ledPin);
    }
}

void led_on(void)
{
    GPIO_ResetBits(g_ledCfg.ledPort, g_ledCfg.ledPin);
}

void led_off(void)
{
    GPIO_SetBits(g_ledCfg.ledPort, g_ledCfg.ledPin);
}
