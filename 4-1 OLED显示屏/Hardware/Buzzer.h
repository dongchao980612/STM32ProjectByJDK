#ifndef __BUZZER_H
#define __BUZZER_H

typedef struct{
	/* Buzzer */
	GPIO_TypeDef*       buzzerPort;
	uint32_t            buzzerClock;
	uint16_t            buzzerPin;
} BuzzerCfg_t;



void buzzer_init(void);


void buzzer_trun(void);


void buzzer_on(void);


void buzzer_off(void);

#endif
