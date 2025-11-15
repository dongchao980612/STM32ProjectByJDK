#ifndef __LED_H
#define __LED_H

   /* Led */
typedef struct
{
    GPIO_TypeDef*       ledPort;
    uint32_t            ledClock;
    uint16_t            ledPin;
} LedCfg_t;



void Led_Init(void);
void Led_On(void);
void Led_Off(void);
void Led_Turn(void);

#endif
