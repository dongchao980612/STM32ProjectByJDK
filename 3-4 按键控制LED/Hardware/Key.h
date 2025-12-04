#ifndef __KEY_H
#define __KEY_H


typedef struct
{
    /* Key */
    GPIO_TypeDef*       keyPort;
    uint32_t            keyClock;
    uint16_t            keyPin;
} KeyCfg_t;

typedef enum
{
    KeyMode_PREDDED = 0,
    KeyMode_RELEASED = 1,
} KeyMode_TypeDef;


typedef enum
{
    NOKEY = -1,
    KEY0 = 0,
    KEY1 = 1,
} KeyNo_TypeDef;


void Key1_Init(void);
void Key0_Init(void);

KeyMode_TypeDef Key0_GetStatus(void);
KeyMode_TypeDef Key1_GetStatus(void);
#endif
