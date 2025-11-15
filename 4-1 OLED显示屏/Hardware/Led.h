#ifndef __LED_H
#define __LED_H


typedef struct
{
    /* Led */
    GPIO_TypeDef*       ledPort;
    uint32_t            ledClock;
    uint16_t            ledPin;
} LedCfg_t;



void led_init(void);
void led_on(void);
void led_off(void);
void led_turn(void);

#endif
