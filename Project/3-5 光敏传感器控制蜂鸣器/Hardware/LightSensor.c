#include "stm32f10x.h"                  // Device header

typedef struct{
	/* LightSensor */
	GPIO_TypeDef*       lightSensorPort;
	uint32_t            lightSensorClock;
	uint16_t            lightSensorPin;
} lightSensorCfg_t;


static lightSensorCfg_t g_lightSensorCfg = {
	/* LightSensor */
	GPIOC,
	RCC_APB2Periph_GPIOC,
	GPIO_Pin_11,
};

void LightSensor_Init(void){
	RCC_APB2PeriphClockCmd(g_lightSensorCfg.lightSensorClock,ENABLE); 

	GPIO_InitTypeDef GPIO_InitStructure;	 
				 
	GPIO_InitStructure.GPIO_Pin =  g_lightSensorCfg.lightSensorPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
	GPIO_Init(g_lightSensorCfg.lightSensorPort, &GPIO_InitStructure);
	
	GPIO_SetBits(g_lightSensorCfg.lightSensorPort,g_lightSensorCfg.lightSensorPin);
}

uint8_t LightSensor_GetNum(void){
	return GPIO_ReadInputDataBit(g_lightSensorCfg.lightSensorPort,g_lightSensorCfg.lightSensorPin);
}
