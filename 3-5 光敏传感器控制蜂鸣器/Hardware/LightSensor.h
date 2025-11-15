#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H

/* LightSensor */
typedef struct
{

    GPIO_TypeDef*       lightSensorPort;
    uint32_t            lightSensorClock;
    uint16_t            lightSensorPin;
} LightSensorCfg_t;


void LightSensor_Init(void);
uint8_t LightSensor_GetNum(void);

#endif
