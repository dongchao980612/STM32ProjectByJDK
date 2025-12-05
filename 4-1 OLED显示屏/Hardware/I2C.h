#ifndef __I2C_H
#define __I2C_H

typedef struct
{
  /* SCL Pin */
  uint32_t sclClock;
  uint16_t sclPin;
  GPIO_TypeDef *sclPort;

  /* SDA Pin */
  uint32_t sdaClock;
  uint16_t sdaPin;
  GPIO_TypeDef *sdaPort;

} I2CCfg_t;




void My_I2C_W_SCL(uint8_t  action);
void My_I2C_W_SDA(uint8_t  action);


void My_I2C_Init(void);

void My_I2C_Start(void);
void My_I2C_Stop(void);

void My_I2C_SendByte(uint8_t Byte);

#endif
