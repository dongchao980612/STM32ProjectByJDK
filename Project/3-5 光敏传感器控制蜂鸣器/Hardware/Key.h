#ifndef __KEY_H
#define __KEY_H


typedef struct{
	/* Key */
	GPIO_TypeDef*       keyPort;
	uint32_t            keyClock;
	uint16_t            keyPin;
} KeyCfg_t;


void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
