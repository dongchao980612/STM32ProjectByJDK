#ifndef __LIGHTSENSOR_H
#define __LIGHTSENSOR_H


typedef struct
{
    /* LightSensor */
    GPIO_TypeDef*       lightSensorPort;
    uint32_t            lightSensorClock;
    uint16_t            lightSensorPin;
} LightSensorCfg_t;

typedef enum
{
    LIGHT_STATUS_BRIGHT = 0,      /* 环境亮 */
    LIGHT_STATUS_DARK = 1   /* 环境暗 */
} LightStatus_TypeDef;


void LightSensor_Init(void);
LightStatus_TypeDef  LightSensor_GetStatus(void);

#endif
