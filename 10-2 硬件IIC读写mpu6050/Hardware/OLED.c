#include "stm32f10x.h"
#include "oled.h"
#include "oled_font.h"
#include "delay.h"
#include "iic.h"

OLEDCfg_t g_oledCfg =
{
    I2C1,

};


void OLED_Init(void)
{
    I2C_Common_Init(g_oledCfg.iic);
    // OLED初始化命令序列
    OLED_WriteCommand(0xAE); // 关闭显示

    OLED_WriteCommand(0xD5); // 设置显示时钟分频比/振荡器频率
    OLED_WriteCommand(0x80);

    OLED_WriteCommand(0xA8); // 设置多路复用率
    OLED_WriteCommand(0x3F); // 64-1

    OLED_WriteCommand(0xD3); // 设置显示偏移
    OLED_WriteCommand(0x00); // 不偏移

    OLED_WriteCommand(0x40); // 设置显示开始行

    OLED_WriteCommand(0xA1); // 设置左右方向，0xA1正常

    OLED_WriteCommand(0xC8); // 设置上下方向，0xC8正常

    OLED_WriteCommand(0xDA); // 设置COM引脚硬件配置
    OLED_WriteCommand(0x12);

    OLED_WriteCommand(0x81); // 设置对比度控制
    OLED_WriteCommand(0xCF);

    OLED_WriteCommand(0xD9); // 设置预充电周期
    OLED_WriteCommand(0xF1);

    OLED_WriteCommand(0xDB); // 设置VCOMH取消选择级别
    OLED_WriteCommand(0x30);

    OLED_WriteCommand(0xA4); // 设置整个显示打开/关闭

    OLED_WriteCommand(0xA6); // 设置正常显示

    OLED_WriteCommand(0x8D); // 设置充电泵
    OLED_WriteCommand(0x14);

    OLED_WriteCommand(0xAF); // 开启显示

    OLED_Clear(); // 清屏
}



void OLED_WriteCommand(uint8_t Command)
{
    // 生成起始条件
    I2C_GenerateSTART(g_oledCfg.iic, ENABLE);
    
    // 等待EV5：起始条件已发送
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
    
    // 发送从机地址（写模式）
    I2C_Send7bitAddress(g_oledCfg.iic, OLED_ADDRESS, I2C_Direction_Transmitter);
    
    // 等待EV6：地址已发送并收到ACK
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
    
    // 发送控制字节（命令模式）
    I2C_SendData(g_oledCfg.iic, OLED_COMMAND_MODE);  // 通常0x00表示命令模式
    
    // 等待EV8_1：数据寄存器为空（可以发送下一个字节）
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);
    
    // 发送具体命令
    I2C_SendData(g_oledCfg.iic, Command);
    
    // 等待EV8_2：数据字节已发送
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);
    
    // 生成停止条件
    I2C_GenerateSTOP(g_oledCfg.iic, ENABLE);
    
    // 添加小延时确保停止条件完成
    Delay_us(10);
}

void OLED_WriteData(uint8_t Data)
{
    // 生成起始条件
    I2C_GenerateSTART(g_oledCfg.iic, ENABLE);
    
    // 等待EV5
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_MODE_SELECT)!=SUCCESS);
    
    // 发送从机地址
    I2C_Send7bitAddress(g_oledCfg.iic, OLED_ADDRESS, I2C_Direction_Transmitter);
    
    // 等待EV6
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)!=SUCCESS);
    
    // 发送控制字节（数据模式）
    I2C_SendData(g_oledCfg.iic, OLED_DATA_MODE);  // 通常0x40表示数据模式
    
    // 等待EV8_1
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTING)!=SUCCESS);
    
    // 发送数据
    I2C_SendData(g_oledCfg.iic, Data);
    
    // 等待EV8_2
    while(I2C_CheckEvent(g_oledCfg.iic, I2C_EVENT_MASTER_BYTE_TRANSMITTED)!=SUCCESS);
    
    // 生成停止条件
    I2C_GenerateSTOP(g_oledCfg.iic, ENABLE);
}



void OLED_SetCursor(uint8_t Y, uint8_t X)
{
    OLED_WriteCommand(OLED_CMD_SET_PAGE_ADDR | (Y & 0x07));       // 设置页地址
    OLED_WriteCommand(OLED_CMD_SET_COL_HIGH | ((X >> 4) & 0x0F)); // 设置列地址高4位
    OLED_WriteCommand(OLED_CMD_SET_COL_LOW | (X & 0x0F));         // 设置列地址低4位
}

void OLED_Clear(void)
{
    uint8_t page, col;

    for (page = 0; page < OLED_PAGE_NUM; page++)
    {
        OLED_SetCursor(page, 0);

        for (col = 0; col < OLED_WIDTH; col++)
        {
            OLED_WriteData(0x00); // 写0清空像素
        }
    }
}

void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char)
{
    uint8_t i;

    // 计算实际坐标
    // Line: 1-4，每行2页(16像素)
    // Column: 1-16，每列8像素
    uint8_t page = (Line - 1) * 2;
    uint8_t col = (Column - 1) * 8;

    // 显示上半部分(前8个字节)
    OLED_SetCursor(page, col);

    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i]);
    }

    // 显示下半部分(后8个字节)
    OLED_SetCursor(page + 1, col);

    for (i = 0; i < 8; i++)
    {
        OLED_WriteData(OLED_F8x16[Char - ' '][i + 8]);
    }
}

/**
 * @brief  OLED显示字符串
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  String 要显示的字符串，范围：ASCII可见字符
 * @retval 无
 */
void OLED_ShowString(uint8_t Line, uint8_t Column, char *String)
{
    uint8_t i;

    for (i = 0; String[i] != '\0'; i++)
    {
        OLED_ShowChar(Line, Column + i, String[i]);
    }
}
/**
 * @brief  OLED次方函数
 * @retval 返回值等于X的Y次方
 */
uint32_t OLED_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;

    while (Y--)
    {
        Result *= X;
    }

    return Result;
}

/**
 * @brief  OLED显示数字（十进制，正数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~4294967295
 * @param  Length 要显示数字的长度，范围：1~10
 * @retval 无
 */
void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;

    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  OLED显示数字（十进制，带符号数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：-2147483648~2147483647
 * @param  Length 要显示数字的长度，范围：1~10
 * @retval 无
 */
void OLED_ShowSignedNum(uint8_t Line, uint8_t Column, int32_t Number, uint8_t Length)
{
    uint8_t i;
    uint32_t Number1;

    if (Number >= 0)
    {
        OLED_ShowChar(Line, Column, '+');
        Number1 = Number;
    }
    else
    {
        OLED_ShowChar(Line, Column, '-');
        Number1 = -Number;
    }

    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i + 1, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/**
 * @brief  OLED显示数字（十六进制，正数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~0xFFFFFFFF
 * @param  Length 要显示数字的长度，范围：1~8
 * @retval 无
 */
void OLED_ShowHexNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i, SingleNumber;

    for (i = 0; i < Length; i++)
    {
        SingleNumber = Number / OLED_Pow(16, Length - i - 1) % 16;

        if (SingleNumber < 10)
        {
            OLED_ShowChar(Line, Column + i, SingleNumber + '0');
        }
        else
        {
            OLED_ShowChar(Line, Column + i, SingleNumber - 10 + 'A');
        }
    }
}

/**
 * @brief  OLED显示数字（二进制，正数）
 * @param  Line 起始行位置，范围：1~4
 * @param  Column 起始列位置，范围：1~16
 * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
 * @param  Length 要显示数字的长度，范围：1~16
 * @retval 无
 */
void OLED_ShowBinNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length)
{
    uint8_t i;

    for (i = 0; i < Length; i++)
    {
        OLED_ShowChar(Line, Column + i, Number / OLED_Pow(2, Length - i - 1) % 2 + '0');
    }
}
