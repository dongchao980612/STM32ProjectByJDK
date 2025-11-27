#ifndef __OLED_H
#define __OLED_H

typedef struct
{
  /* SCL Pin */
  uint32_t             sclClock;
  uint16_t             sclPin;
  GPIO_TypeDef*        sclPort;
	
  /* SDA Pin */
  uint32_t             sdaClock;
  uint16_t             sdaPin;
  GPIO_TypeDef*        sdaPort;
} OledCfg_t;


void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length);
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);

#endif
