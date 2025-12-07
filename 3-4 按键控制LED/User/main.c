#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"
#include "key.h"

// LED≈‰÷√
LedCfg_t g_ledCfg0 =
{
    /* Led0 */
    GPIOB,
    RCC_APB2Periph_GPIOB,
    GPIO_Pin_5
};

LedCfg_t g_ledCfg1 =
{
    /* Led1 */
    GPIOE,
    RCC_APB2Periph_GPIOE,
    GPIO_Pin_5
};


// ∞¥º¸≈‰÷√
KeyCfg_t g_keyCfgs0 =
{
    /* Key0 */
    GPIOE,
    RCC_APB2Periph_GPIOE,
    GPIO_Pin_4
};

KeyCfg_t g_keyCfgs1 =
{
    /* Key1 */
    GPIOE,
    RCC_APB2Periph_GPIOE,
    GPIO_Pin_3
};


int main(void)
{
    Led0_Init();
    Led1_Init();

    Key0_Init();
    Key1_Init();

    // Led0_On();
    // Led1_On();


    while (1)
    {
        // Led0_On();
        // Delay_ms(1000);
        // Led0_Off();
        // Delay_ms(1000);
        #if  1
        if(Key0_GetStatus() == KeyMode_PREDDED)
        {
            Led0_On();
        }
        else
        {
            Led0_Off();
        }



        if(Key1_GetStatus() == KeyMode_PREDDED)
        {

            Led1_On();
        }
        else
        {

            Led1_Off();
        }


        #endif

    }
}
