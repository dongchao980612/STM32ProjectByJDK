#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "key.h"

// 按键配置
static KeyCfg_t g_keyCfgs[] =
{
    {GPIOE, RCC_APB2Periph_GPIOE, GPIO_Pin_4, GPIO_Mode_IPU}, // KEY0 通常上拉
    {GPIOE, RCC_APB2Periph_GPIOE, GPIO_Pin_3, GPIO_Mode_IPU}, // KEY1 通常上拉
    {GPIOA, RCC_APB2Periph_GPIOA, GPIO_Pin_0, GPIO_Mode_IPD}, // 根据KEY_UP硬件调整
};


const int SELECT_KEY = 2;

// 按键初始化
void key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(g_keyCfgs[SELECT_KEY].keyClock, ENABLE);
    GPIO_InitStructure.GPIO_Pin  = g_keyCfgs[SELECT_KEY].keyPin;
    GPIO_InitStructure.GPIO_Mode = g_keyCfgs[SELECT_KEY].keyMode;  // 上拉输入
    GPIO_Init(g_keyCfgs[SELECT_KEY].keyPort, &GPIO_InitStructure);
}

uint8_t key_getNum(void)
{
    uint8_t keyNum = 0;

    if(GPIO_ReadInputDataBit(g_keyCfgs[SELECT_KEY].keyPort, g_keyCfgs[SELECT_KEY].keyPin) == 0) //按下
    {
        delay_ms(20);

        while(GPIO_ReadInputDataBit(g_keyCfgs[SELECT_KEY].keyPort, g_keyCfgs[SELECT_KEY].keyPin) == 0)
        {
            delay_ms(20);
            keyNum = 1;
        }
    }

    return keyNum;
}
