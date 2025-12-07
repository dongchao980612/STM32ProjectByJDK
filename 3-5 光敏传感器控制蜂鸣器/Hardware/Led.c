#include "stm32f10x.h"                  // Device header
#include "led.h"

extern LedCfg_t g_ledCfg0;
extern LedCfg_t g_ledCfg1;

void Led0_Init(void)
{
    RCC_APB2PeriphClockCmd(g_ledCfg0.ledClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin =  g_ledCfg0.ledPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(g_ledCfg0.ledPort, &GPIO_InitStructure);

    GPIO_SetBits(g_ledCfg0.ledPort, g_ledCfg0.ledPin);
}

void Led0_Turn()
{
    if(GPIO_ReadOutputDataBit(g_ledCfg0.ledPort, g_ledCfg0.ledPin) == LEDMode_ON)
    {
        Led0_Off();
    }
    else
    {
        Led0_On();
    }
}

void Led0_On(void)
{
    GPIO_ResetBits(g_ledCfg0.ledPort, g_ledCfg0.ledPin);
}

void Led0_Off(void)
{
    GPIO_SetBits(g_ledCfg0.ledPort, g_ledCfg0.ledPin);
}





void Led1_Init(void)
{
    RCC_APB2PeriphClockCmd(g_ledCfg1.ledClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin =  g_ledCfg0.ledPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(g_ledCfg1.ledPort, &GPIO_InitStructure);

    GPIO_SetBits(g_ledCfg1.ledPort, g_ledCfg1.ledPin);
}

void Led1_Turn()
{
    if(GPIO_ReadOutputDataBit(g_ledCfg1.ledPort, g_ledCfg1.ledPin) == LEDMode_ON)
    {
        Led1_Off();
    }
    else
    {
        Led1_On();
    }
}

void Led1_On(void)
{
    GPIO_ResetBits(g_ledCfg1.ledPort, g_ledCfg1.ledPin);
}

void Led1_Off(void)
{
    GPIO_SetBits(g_ledCfg1.ledPort, g_ledCfg1.ledPin);
}
