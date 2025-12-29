#ifndef __IIC_H
#define __IIC_H


void I2C_Common_Init(I2C_TypeDef* I2Cx);

void I2C_TimeOutEvent(I2C_TypeDef* I2Cx, uint32_t I2C_EVENT);


#endif
