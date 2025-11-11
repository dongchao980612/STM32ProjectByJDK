#include "stm32f10x.h"                  // Device header
#include "delay.h"

typedef struct
{
    /* Led */
    GPIO_TypeDef*       ledPort;
    uint32_t            ledClock;
    uint16_t            ledPin;
    uint32_t						ledDelay;
} LedCfg_t;

static LedCfg_t g_ledCfg =
{
    /* Led */
    GPIOB,
    RCC_APB2Periph_GPIOB,
    GPIO_Pin_5,
    500
};



int main(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(g_ledCfg.ledClock, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  g_ledCfg.ledPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_Init(g_ledCfg.ledPort, &GPIO_InitStructure);
	
	
    // 点亮LED
    // GPIO_ResetBits(g_ledCfg.ledPort,g_ledCfg.ledPin); // high ensable led
    // GPIO_WriteBit(g_ledCfg.ledPort,g_ledCfg.ledPin,Bit_RESET);// Bit_RESET->1 Bit_SET->0

    while (1)
    {
        // LED闪烁
        // GPIO_WriteBit(g_ledCfg.ledPort,g_ledCfg.ledPin,Bit_RESET); // Bit_RESET->1  enable led
        // Delay_ms(g_ledCfg.ledDelay);
        // GPIO_WriteBit(g_ledCfg.ledPort,g_ledCfg.ledPin,Bit_SET); // Bit_SET->0  disable led
        // Delay_ms(g_ledCfg.ledDelay);

        // GPIO_SetBits(g_ledCfg.ledPort, g_ledCfg.ledPin); // low disable led
        // Delay_ms(g_ledCfg.ledDelay);
				// GPIO_ResetBits(g_ledCfg.ledPort,g_ledCfg.ledPin); // high ensable led
				// Delay_ms(g_ledCfg.ledDelay);
    }
}
