#ifndef __ENCODER_H
#define __ENCODER_H

typedef struct{
	/* Encoder */
	GPIO_TypeDef*       encoderPort;
	uint32_t            encoderClock;
	uint16_t            encoderPin1;
	uint16_t          	encoderPin2;

	/* AFIO */
	uint8_t         		afioPortSource;
	uint32_t            afioClock;
	uint8_t             afioPinSource1;
	uint8_t             afioPinSource2;

	/* EXTI */
  uint32_t 						exitLine1;   
  uint32_t 						exitLine2;  	

	/* NVIC*/
	uint32_t  					nvicPriorityGroup;
  uint8_t 						nvicIRQChannel1;  
  uint8_t 						nvicIRQChannel2; 	
} EncoderCfg_t;


void Encoder_Init(void);
int16_t Encoder_Get(void);

#endif
