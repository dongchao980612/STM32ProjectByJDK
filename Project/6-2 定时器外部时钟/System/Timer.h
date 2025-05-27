#ifndef __TIMER_H
#define __TIMER_H

void Timer2_Init(void);
void TIM2_IRQHandler(void);
uint16_t Timer2_GetCounter(void);

#endif
