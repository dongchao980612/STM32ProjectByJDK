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

#define I2C_ACK 0             // 应答信号（从机拉低SDA）
#define I2C_NACK 1            // 非应答信号（从机拉高SDA）

void My_I2C_Delay(void);

void My_I2C_W_SCL(uint8_t  action);
void My_I2C_W_SDA(uint8_t  action);
uint8_t My_I2C_R_SDA(void);


void My_I2C_Init(void);

void My_I2C_Start(void);
void My_I2C_Stop(void);

void My_I2C_SendByte(uint8_t Byte);
uint8_t My_I2C_ReceiveByte(void);

uint8_t My_I2C_ReceiveACK(void);
void My_I2C_SendACK(uint8_t ACKBit);

#endif
