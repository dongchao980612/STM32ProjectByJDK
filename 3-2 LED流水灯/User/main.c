#include "stm32f10x.h"                  // Device header
#include "delay.h"

typedef struct
{
    GPIO_TypeDef*       ledPort_B;
    GPIO_TypeDef*       ledPort_E;
    uint32_t            ledClock_B;
    uint32_t            ledClock_E;
    uint16_t            ledPin;
    uint32_t			      ledDelay;
} LedCfg_t;

static LedCfg_t g_ledCfg =
{
    GPIOB,
    GPIOE,
    RCC_APB2Periph_GPIOB,
    RCC_APB2Periph_GPIOE,
    GPIO_Pin_5,
    1000
};

int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(g_ledCfg.ledClock_B, ENABLE);
    RCC_APB2PeriphClockCmd(g_ledCfg.ledClock_E, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  g_ledCfg.ledPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(g_ledCfg.ledPort_B, &GPIO_InitStructure);
    GPIO_Init(g_ledCfg.ledPort_E, &GPIO_InitStructure);

    while (1)
    {
        // µ„¡¡LED0£®PB.5£©£¨œ®√LED1£®PE.5£©
        GPIO_ResetBits(g_ledCfg.ledPort_B, g_ledCfg.ledPin);
        GPIO_SetBits(g_ledCfg.ledPort_E, g_ledCfg.ledPin);
        Delay_ms(g_ledCfg.ledDelay);

        // œ®√LED0£®PB.5£©£¨µ„¡¡LED1£®PE.5£©
        GPIO_SetBits(g_ledCfg.ledPort_B, g_ledCfg.ledPin);
        GPIO_ResetBits(g_ledCfg.ledPort_E, g_ledCfg.ledPin);
        Delay_ms(g_ledCfg.ledDelay);
    }
}
