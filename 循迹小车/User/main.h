/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains common defines and function prototypes
  *                   used in the project.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 [Your Name/Organization]
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"  // 标准库核心头文件（根据芯片型号选择，如stm32f10x.h/stm32f4xx.h等）
#include <stdint.h>      // 标准整数类型定义（uint8_t, int32_t等）
#include <stdbool.h>     // 布尔类型定义（bool, true, false）

/* Exported constants --------------------------------------------------------*/
/* 系统时钟频率定义（根据实际配置修改，此处以72MHz为例） */
#define SYSCLK_FREQ    72000000UL  // 系统时钟频率
#define HCLK_FREQ      SYSCLK_FREQ // AHB总线时钟频率
#define PCLK1_FREQ     (SYSCLK_FREQ / 2)  // APB1总线时钟频率（低速）
#define PCLK2_FREQ     SYSCLK_FREQ  // APB2总线时钟频率（高速）

/* 通用错误码定义 */
#define ERROR_OK       0   // 操作成功
#define ERROR_TIMEOUT  1   // 超时错误
#define ERROR_PARAM    2   // 参数错误

/* Exported macro ------------------------------------------------------------*/
/* 延时相关宏（需配合SysTick使用） */
#define delay_us(us)   SysTick_Delay_us(us)   // 微秒级延时（声明）
#define delay_ms(ms)   SysTick_Delay_ms(ms)   // 毫秒级延时（声明）

/* 位操作宏 */
#define SET_BIT(reg, bit)    ((reg) |= (bit))
#define CLEAR_BIT(reg, bit)  ((reg) &= ~(bit))
#define TOGGLE_BIT(reg, bit) ((reg) ^= (bit))
#define CHECK_BIT(reg, bit)  ((reg) & (bit)) ? 1 : 0

/* Exported functions prototypes ---------------------------------------------*/
/* 系统初始化函数 */
void System_Init(void);                  // 系统总初始化（含时钟、外设等）
void SysTick_Configuration(void);        // SysTick定时器配置（用于延时）
void SysTick_Delay_us(uint32_t us);      // 微秒延时实现
void SysTick_Delay_ms(uint32_t ms);      // 毫秒延时实现

/* 错误处理函数 */
void Error_Handler(uint8_t error_code);  // 错误处理（替代HAL库的Error_Handler）

/* USER CODE BEGIN EFP */
#define fabs(x)     (((x)>0)?(x):-(x));
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* 芯片型号确认（根据实际使用的芯片修改，标准库需要明确型号） */
#define STM32F103xE  // 例如：STM32F103C8T6对应STM32F103x8，STM32F103ZET6对应STM32F103xE

/* USER CODE BEGIN Private defines */
// 此处可添加用户自定义的宏定义（如引脚别名、硬件参数等）
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */