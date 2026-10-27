#ifndef _STM32F407G_DISC1_RCC_H_
#define _STM32F407G_DISC1_RCC_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

// Crystal frequencies on STM32F407G-DISC1
#define HSI_VALUE                   16000000u   // 16 MHz Internal RC
#define HSE_VALUE                   8000000u    // 8 MHz Onboard Crystal

typedef struct{
    uint8_t  RCC_ClockSource;       // HSI, HSE, or PLL
    uint8_t  RCC_PLLSource;         // HSI or HSE as PLL input
    uint8_t  RCC_PLLM;              // Division factor for main PLL input clock (2..63)
    uint16_t RCC_PLLN;              // Main PLL multiplication factor for VCO (50..432)
    uint8_t  RCC_PLLP;              // Main PLL division factor for main system clock (2, 4, 6, 8)
    uint8_t  RCC_AHBPrescaler;      // HCLK prescaler
    uint8_t  RCC_APB1Prescaler;     // PCLK1 prescaler (Max 42 MHz)
    uint8_t  RCC_APB2Prescaler;     // PCLK2 prescaler (Max 84 MHz)
    uint8_t  RCC_FlashLatency;      // Flash wait states (0..5 WS)
}RCC_Config_t;

typedef struct{
    RCC_RegDef_t* pRCC;
    RCC_Config_t  RCC_Config;
}RCC_Handle_t;

//RCC_ClockSource (SYSCLK Source)
#define RCC_SYSCLK_HSI              0
#define RCC_SYSCLK_HSE              1
#define RCC_SYSCLK_PLL              2

//RCC_PLLSource
#define RCC_PLL_SOURCE_HSI          0
#define RCC_PLL_SOURCE_HSE          1

//RCC_PLLP (Bit values for PLLCFGR[17:16])
#define RCC_PLLP_DIV2               0
#define RCC_PLLP_DIV4               1
#define RCC_PLLP_DIV6               2
#define RCC_PLLP_DIV8               3

//RCC_AHBPrescaler (CFGR[7:4] HPRE)
#define RCC_AHB_DIV1                0
#define RCC_AHB_DIV2                8
#define RCC_AHB_DIV4                9
#define RCC_AHB_DIV8                10
#define RCC_AHB_DIV16               11
#define RCC_AHB_DIV64               12
#define RCC_AHB_DIV128              13
#define RCC_AHB_DIV256              14
#define RCC_AHB_DIV512              15

//RCC_APB1Prescaler & RCC_APB2Prescaler (CFGR[12:10] PPRE1 & CFGR[15:13] PPRE2)
#define RCC_APB_DIV1                0
#define RCC_APB_DIV2                4
#define RCC_APB_DIV4                5
#define RCC_APB_DIV8                6
#define RCC_APB_DIV16               7

//RCC_FlashLatency (Wait States at 3.3V VDD)
#define RCC_FLASH_LATENCY_0WS       0   // 0 < HCLK <= 30 MHz
#define RCC_FLASH_LATENCY_1WS       1   // 30 < HCLK <= 60 MHz
#define RCC_FLASH_LATENCY_2WS       2   // 60 < HCLK <= 90 MHz
#define RCC_FLASH_LATENCY_3WS       3   // 90 < HCLK <= 120 MHz
#define RCC_FLASH_LATENCY_4WS       4   // 120 < HCLK <= 150 MHz
#define RCC_FLASH_LATENCY_5WS       5   // 150 < HCLK <= 168 MHz

void RCC_Init(RCC_Handle_t* pRCCHandle);

uint32_t RCC_GetSYSCLKValue(void);
uint32_t RCC_GetHCLKValue(void);
uint32_t RCC_GetPCLK1Value(void);
uint32_t RCC_GetPCLK2Value(void);

#endif