#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"                  // Device header

void My_USART_Init(void);
void My_USART_SendByte(uint8_t Byte);
void My_USART_SendArray(uint8_t *Array, uint16_t Length);
void My_USART_SendString(char *String);
void My_USART_SendNumber(uint32_t Number, uint8_t Length);
void My_Printf(char *format, ...);


uint8_t USART_GetRxFlag(void);
uint8_t USART_GetRxData(void);


#endif

