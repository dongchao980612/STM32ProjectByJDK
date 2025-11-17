#ifndef __TIMER_H
#define __TIMER_H

typedef struct
{
    /* Timer2 */
    TIM_TypeDef* 					timer;
    uint32_t 						timerClock;

    uint16_t 						timerClockDivision;
    uint16_t 						timerCounterMode;

    uint16_t 						timerPeriod;
    uint16_t 						timerPrescaler;
    uint16_t 						timerRepetitionCounter;


    /* NVIC*/
    uint32_t  					nvicPriorityGroup;
    uint8_t 						nvicIRQChannel;
} timer2Cfg_t;

void Timer_Init(void);
uint16_t Timer2_GetCounter(void);


#endif
