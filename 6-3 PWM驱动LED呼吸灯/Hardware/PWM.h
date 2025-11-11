#ifndef __PWM_H
#define __PWM_H

typedef struct{
	/* PWM */
	GPIO_TypeDef*       pwmPort;
	uint32_t            pwmClock;
	uint16_t            pwmPin;
	
	/* Timer2 */
	TIM_TypeDef* 				timer;
	uint32_t 						timerClock;

	uint16_t 						timerClockDivision;
	uint16_t 						timerCounterMode;

	uint16_t 						timerPeriod;
	uint16_t 						timerPrescaler;
	uint16_t 						timerRepetitionCounter;
	uint16_t 						timerPulse;
} pwmCfg_t;



void PWM_Init(void);
void PWM_SetCompare1(uint16_t Compare);

#endif
