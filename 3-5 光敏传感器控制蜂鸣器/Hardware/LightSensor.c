#include "stm32f10x.h"                  // Device header
#include "lightsensor.h"

extern  LightSensorCfg_t g_lightSensorCfg;

void LightSensor_Init(void)
{
    RCC_APB2PeriphClockCmd(g_lightSensorCfg.lightSensorClock, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin =  g_lightSensorCfg.lightSensorPin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(g_lightSensorCfg.lightSensorPort, &GPIO_InitStructure);

    GPIO_SetBits(g_lightSensorCfg.lightSensorPort, g_lightSensorCfg.lightSensorPin);
}

LightStatus_TypeDef  LightSensor_GetStatus(void)
{
    uint8_t raw = GPIO_ReadInputDataBit(g_lightSensorCfg.lightSensorPort, g_lightSensorCfg.lightSensorPin);

    return (raw ? LIGHT_STATUS_DARK : LIGHT_STATUS_BRIGHT);
}
