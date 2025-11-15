#ifndef __BUZZER_H
#define __BUZZER_H

typedef struct{
	/* Buzzer */
	GPIO_TypeDef*       buzzerPort;
	uint32_t            buzzerClock;
	uint16_t            buzzerPin;
} BuzzerCfg_t;



void Buzzer_Init(void);


void Buzzer_Trun(void);


void Buzzer_On(void);


void Buzzer_Off(void);

#endif
