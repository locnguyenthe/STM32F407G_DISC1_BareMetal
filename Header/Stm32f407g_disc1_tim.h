#ifndef _STM32F407G_DISC1_TIM_H_
#define _STM32F407G_DISC1_TIM_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

typedef struct{
    uint16_t TIM_Prescaler;
    uint32_t TIM_Period;
    uint8_t  TIM_CounterMode;
    uint8_t  TIM_AutoReloadPreload;
    uint8_t  TIM_ClockSource;
    uint8_t  TIM_ExtTriggerSource;
    uint8_t  TIM_ExtTriggerPolarity;
}TIM_Config_t;

typedef struct{
    TIM_RegDef_t* pTIMx;
    TIM_Config_t  TIM_Config;
}TIM_Handle_t;

//TIM_CounterMode
#define TIM_COUNTER_UP                  0
#define TIM_COUNTER_DOWN                1
#define TIM_COUNTER_CENTER_ALIGNED1     2
#define TIM_COUNTER_CENTER_ALIGNED2     3
#define TIM_COUNTER_CENTER_ALIGNED3     4

//TIM_AutoReloadPreload
#define TIM_ARP_DISABLE                 0
#define TIM_ARP_ENABLE                  1

//TIM_ClockSource
#define TIM_CLK_INTERNAL                0
#define TIM_CLK_EXT_MODE1               1   // External input pin (TI1/TI2/ETR via SMCR SMS=111)
#define TIM_CLK_EXT_MODE2               2   // External trigger input ETR (SMCR ECE=1)

//TIM_ExtTriggerSource (Used when TIM_ClockSource == TIM_CLK_EXT_MODE1)
#define TIM_TS_TI1F_ED                  4   // TI1 Edge Detector
#define TIM_TS_TI1FP1                   5   // Filtered Timer Input 1
#define TIM_TS_TI2FP2                   6   // Filtered Timer Input 2
#define TIM_TS_ETRF                     7   // External Trigger Input

//TIM_ExtTriggerPolarity
#define TIM_ETP_RISING                  0   // Non-inverted / rising edge
#define TIM_ETP_FALLING                 1   // Inverted / falling edge

void TIM_CLK_Control(TIM_RegDef_t* pTIMx, uint8_t EnorDi);

void TIM_Init(TIM_Handle_t* pTIMHandle);
void TIM_Reset(TIM_RegDef_t* pTIMx);

void TIM_Start(TIM_RegDef_t* pTIMx, uint8_t EnorDi);
uint32_t TIM_ReadCounter(TIM_RegDef_t* pTIMx);
void TIM_WriteCounter(TIM_RegDef_t* pTIMx, uint32_t value);

uint8_t TIM_GetUpdateFlag(TIM_RegDef_t* pTIMx);
void TIM_ClearUpdateFlag(TIM_RegDef_t* pTIMx);

void TIM_IRQInterrupt_Enable(TIM_RegDef_t* pTIMx, uint8_t EnorDi);
void TIM_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void TIM_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);
void TIM_IRQInterrupt_Handling(TIM_RegDef_t* pTIMx);

#endif