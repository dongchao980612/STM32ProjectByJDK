#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "led.h"

#include "buzzer.h"
#include "lightsensor.h"



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

// ∑‰√˘∆˜≈‰÷√
BuzzerCfg_t g_buzzerCfg =
{
    /* Buzzer */
    GPIOB,
    RCC_APB2Periph_GPIOB,
    GPIO_Pin_8,
};


//  ¥´∏–∆˜≈‰÷√
LightSensorCfg_t g_lightSensorCfg =
{
    /* LightSensor */
    GPIOF,
    RCC_APB2Periph_GPIOF,
    GPIO_Pin_8,
};


int main(void)
{
    Led0_Init();
    Buzzer_Init();
    LightSensor_Init();

    while (1)
    {

        if(LightSensor_GetStatus() == LIGHT_STATUS_BRIGHT)
        {
            Buzzer_Off();
            Led0_Off();
        }
        else
        {
            Buzzer_On();
            Led0_On();
        }
    }
}
