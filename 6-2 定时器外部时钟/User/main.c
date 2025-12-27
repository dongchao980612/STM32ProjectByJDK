#include "stm32f10x.h"                  // Device header
#include "oled.h"
#include "timer.h"

#include "i2c.h"

I2CCfg_t g_i2cCfg =
{
    /* SCL Pin*/
    RCC_APB2Periph_GPIOB,
    GPIO_Pin_6,
    GPIOB,

    /* SDA Pin */
    RCC_APB2Periph_GPIOB,
    GPIO_Pin_7,
    GPIOB,
};

uint32_t  Number = 0;

int main(void)
{
    OLED_Init();		//OLED初始化
    Timer_Init();

    OLED_ShowString(1, 1, "Number:");
    OLED_ShowString(2, 1, "Counter:");

    while (1)
    {
        OLED_ShowNum(1, 9, Number, 5);
        OLED_ShowNum(2, 9, Timer2_GetCounter(), 5);
    }
}
