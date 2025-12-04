#ifndef __LED_H
#define __LED_H


typedef struct
{
    /* Led */
    GPIO_TypeDef*       ledPort;
    uint32_t            ledClock;
    uint16_t            ledPin;
} LedCfg_t;

typedef enum
{
    LEDMode_ON = 0,
    LEDMode_OFF = 1,
} LEDMode_TypeDef;


// LED0
void Led0_Init(void);
void Led0_On(void);
void Led0_Off(void);
void Led0_Turn(void);


// LED1
void Led1_Init(void);
void Led1_On(void);
void Led1_Off(void);
void Led1_Turn(void);


#endif
