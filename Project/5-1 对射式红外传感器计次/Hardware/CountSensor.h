#ifndef __COUNT_SENSOR_H
#define __COUNT_SENSOR_H


typedef struct{
	/* CountSensor */
	GPIO_TypeDef*       countsensorPort;
	uint32_t            countsensorClock;
	uint16_t            countsensorPin;

	/* AFIO */
	uint8_t         		afioPortSource;
	uint32_t            afioClock;
	uint8_t             afioPinSource;

	/* EXTI */
  uint32_t 						exitLine;              

	/* NVIC*/
	uint32_t  					nvicPriorityGroup;
  uint8_t 						nvicIRQChannel;              
} CountSensorCfg_t;


void CountSensor_Init(void);
uint16_t CountSensor_Get(void);

#endif
