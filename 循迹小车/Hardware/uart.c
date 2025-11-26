#include "main.h"
#include "uart.h"
#include "jy61p.h"

void USART_Configuration(void)
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能USART2时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    // 串口参数配置（115200波特率，对应JY61P）
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    // 使能串口接收中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_Cmd(USART2, ENABLE); // 使能USART2
}

// 串口中断服务函数（接收JY61P数据）
void USART2_IRQHandler(void)
{
    uint8_t rx_data;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        rx_data = USART_ReceiveData(USART2); // 读取接收数据
        jy61p_ReceiveData(rx_data); // 调用JY61P解析函数
        USART_ClearITPendingBit(USART2, USART_IT_RXNE); // 清除中断标志
    }
}