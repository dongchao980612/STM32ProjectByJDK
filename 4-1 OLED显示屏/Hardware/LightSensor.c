#include "stm32f10x.h"                  // Device header
#include "lightsensor.h"

/* LightSensor */
static LightSensorCfg_t g_lightSensorCfg =
{
    GPIOF,
    RCC_APB2Periph_GPIOF,
    GPIO_Pin_8,
};

void lightSensor_init(void)
{
    RCC_APB2PeriphClockCmd(g_lightSensorCfg.lightSensorClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin =  g_lightSensorCfg.lightSensorPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(g_lightSensorCfg.lightSensorPort, &GPIO_InitStructure);

    GPIO_SetBits(g_lightSensorCfg.lightSensorPort, g_lightSensorCfg.lightSensorPin);
}

uint8_t lightSensor_getNum(void)
{
    return GPIO_ReadInputDataBit(g_lightSensorCfg.lightSensorPort, g_lightSensorCfg.lightSensorPin);
}
