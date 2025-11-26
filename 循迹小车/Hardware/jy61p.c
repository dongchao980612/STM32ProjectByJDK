#include "main.h"
#include "jy61p.h"


static uint8_t RxBuffer[11];
static uint8_t RxState = 0;
static uint8_t RxIndex = 0;
float Roll, Pitch, Yaw; // 角度数据

void jy61p_ReceiveData(uint8_t RxData)
{
    uint8_t i, sum = 0;

    if (RxState == 0 && RxData == 0x55)   // 包头0x55
    {
        RxBuffer[RxIndex++] = RxData;
        RxState = 1;
    }
    else if (RxState == 1 && RxData == 0x53)     // 数据类型0x53（角度）
    {
        RxBuffer[RxIndex++] = RxData;
        RxState = 2;
    }
    else if (RxState == 2)     // 接收数据
    {
        RxBuffer[RxIndex++] = RxData;

        if (RxIndex == 11)   // 接收完成11字节
        {
            for (i = 0; i < 10; i++)
            {
                sum += RxBuffer[i];    // 校验和
            }

            if (sum == RxBuffer[10])   // 校验通过
            {
                Roll = ((int16_t)(RxBuffer[3] << 8 | RxBuffer[2])) / 32768.0f * 180.0f;
                Pitch = ((int16_t)(RxBuffer[5] << 8 | RxBuffer[4])) / 32768.0f * 180.0f;
                Yaw = ((int16_t)(RxBuffer[7] << 8 | RxBuffer[6])) / 32768.0f * 180.0f;
            }

            RxState = 0;
            RxIndex = 0;
        }
    }
    else     // 异常状态重置
    {
        RxState = 0;
        RxIndex = 0;
    }
}