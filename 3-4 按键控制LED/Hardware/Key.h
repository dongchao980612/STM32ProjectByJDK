#ifndef __KEY_H
#define __KEY_H


typedef struct{
	/* Key */
	GPIO_TypeDef*       keyPort;
	uint32_t            keyClock;
	uint16_t            keyPin;
	GPIOMode_TypeDef             keyMode;
} KeyCfg_t;


void key_init(void);
uint8_t key_getNum(void);

#endif
