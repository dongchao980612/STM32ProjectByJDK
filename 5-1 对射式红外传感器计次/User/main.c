#include "stm32f10x.h"                  // Device header
#include "i2c.h"
#include "oled.h"
#include "countsensor.h"

I2CCfg_t g_i2cCfg =
{
    /* SCL Pin*/
    RCC_APB2Periph_GPIOD,
    GPIO_Pin_3,
    GPIOD,

    /* SDA Pin */
    RCC_APB2Periph_GPIOG,
    GPIO_Pin_13,
    GPIOG,
};


CountSensorCfg_t g_countsensorCfg =
{
    /* CountSensor */
    GPIOC,
    RCC_APB2Periph_GPIOC,
    GPIO_Pin_0,

    /* AFIO */
    GPIO_PortSourceGPIOC,
    RCC_APB2Periph_AFIO,
    GPIO_PinSource0,

    /* EXIT */
    EXTI_Line0,

    /* NVIC */
    NVIC_PriorityGroup_2,
    EXTI0_IRQn
};

int main(void)
{
    /*模块初始化*/
    OLED_Init();		//OLED初始化
    CountSensor_Init();		//计数传感器初始化

    OLED_ShowString(1, 1, "Count number:");	//1行1列显示字符串Count number:


    while (1)
    {
        uint16_t cnt = CountSensor_Get();
        OLED_ShowNum(2, 2, cnt, 3);
    }
}
