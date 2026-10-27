#ifndef _STM32F407G_DISC1_ADC_H_
#define _STM32F407G_DISC1_ADC_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

// ================= REGISTER DEFINITIONS (Added here for modularity) =================
typedef struct{
    __vo uint32_t SR;               // 0x00 Status register
    __vo uint32_t CR1;              // 0x04 Control register 1
    __vo uint32_t CR2;              // 0x08 Control register 2
    __vo uint32_t SMPR1;            // 0x0C Sample time register 1
    __vo uint32_t SMPR2;            // 0x10 Sample time register 2
    __vo uint32_t JOFR1;            // 0x14 Injected channel data offset register 1
    __vo uint32_t JOFR2;            // 0x18 Injected channel data offset register 2
    __vo uint32_t JOFR3;            // 0x1C Injected channel data offset register 3
    __vo uint32_t JOFR4;            // 0x20 Injected channel data offset register 4
    __vo uint32_t HTR;              // 0x24 Watchdog higher threshold register
    __vo uint32_t LTR;              // 0x28 Watchdog lower threshold register
    __vo uint32_t SQR1;             // 0x2C Regular sequence register 1
    __vo uint32_t SQR2;             // 0x30 Regular sequence register 2
    __vo uint32_t SQR3;             // 0x34 Regular sequence register 3
    __vo uint32_t JSQR;             // 0x38 Injected sequence register
    __vo uint32_t JDR1;             // 0x3C Injected data register 1
    __vo uint32_t JDR2;             // 0x40 Injected data register 2
    __vo uint32_t JDR3;             // 0x44 Injected data register 3
    __vo uint32_t JDR4;             // 0x48 Injected data register 4
    __vo uint32_t DR;               // 0x4C Regular data register
}ADC_RegDef_t;

// ADC Base Addresses (APB2)
#define ADC1_Base                   0x40012000u
#define ADC2_Base                   0x40012100u
#define ADC3_Base                   0x40012200u

#define ADC1                        ((ADC_RegDef_t*)ADC1_Base)
#define ADC2                        ((ADC_RegDef_t*)ADC2_Base)
#define ADC3                        ((ADC_RegDef_t*)ADC3_Base)

typedef struct{
    uint8_t ADC_Resolution;
    uint8_t ADC_ContinuousConvMode;
    uint8_t ADC_DataAlign;
}ADC_Config_t;

typedef struct{
    ADC_RegDef_t* pADCx;
    ADC_Config_t  ADC_Config;
}ADC_Handle_t;

//ADC_Resolution (CR1 [25:24])
#define ADC_RES_12BIT               0
#define ADC_RES_10BIT               1
#define ADC_RES_8BIT                2
#define ADC_RES_6BIT                3

//ADC_ContinuousConvMode (CR2 [1])
#define ADC_CONV_SINGLE             0
#define ADC_CONV_CONTINUOUS         1

//ADC_DataAlign (CR2 [11])
#define ADC_ALIGN_RIGHT             0
#define ADC_ALIGN_LEFT              1

void ADC_CLK_Control(ADC_RegDef_t* pADCx, uint8_t EnorDi);

void ADC_Init(ADC_Handle_t* pADCHandle);
void ADC_Reset(ADC_RegDef_t* pADCx);

void ADC_StartConversion(ADC_RegDef_t* pADCx);
uint16_t ADC_ReadValue(ADC_RegDef_t* pADCx);

uint8_t ADC_GetEOCFlag(ADC_RegDef_t* pADCx);
void ADC_ClearEOCFlag(ADC_RegDef_t* pADCx);

void ADC_IRQInterrupt_Enable(ADC_RegDef_t* pADCx, uint8_t EnorDi);
void ADC_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void ADC_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);

#endif