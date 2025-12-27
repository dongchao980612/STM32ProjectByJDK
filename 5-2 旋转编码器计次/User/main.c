#include "stm32f10x.h"                  // Device header

#include "oled.h"
#include "CountSensor.h"
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
