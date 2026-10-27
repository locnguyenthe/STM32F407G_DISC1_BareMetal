#ifndef _STM32F407G_DISC1_SYSTICK_H_
#define _STM32F407G_DISC1_SYSTICK_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

typedef struct{
    uint32_t SYSTICK_ReloadValue;
    uint8_t  SYSTICK_ClockSource;
    uint8_t  SYSTICK_Interrupt;
}SYSTICK_Config_t;

typedef struct{
    SYSTICK_RegDef_t* pSYSTICK;
    SYSTICK_Config_t  SYSTICK_Config;
}SYSTICK_Handle_t;

//SYSTICK_ClockSource
#define SYSTICK_CLK_AHB_DIV8            0
#define SYSTICK_CLK_AHB                 1

//SYSTICK_Interrupt
#define SYSTICK_INT_DISABLE             0
#define SYSTICK_INT_ENABLE              1

//Default System Core Clock (16 MHz HSI)
#define SYSTEM_CORE_CLOCK               16000000u

void SYSTICK_Init(SYSTICK_Handle_t* pSYSTICKHandle);
void SYSTICK_Start(uint8_t EnorDi);

uint8_t  SYSTICK_GetCountFlag(void);
uint32_t SYSTICK_ReadCurrentValue(void);

void SYSTICK_DelayMs(uint32_t ms);
void SYSTICK_DelayUs(uint32_t us);

void SYSTICK_IRQInterrupt_Config(uint8_t EnorDi);

#endif