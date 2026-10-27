#include <stdint.h>
#include "Stm32f407g_disc1_adc.h"

#define SCB_SHPR ((volatile uint8_t*)0xE000ED18)

void ADC_CLK_Control(ADC_RegDef_t* pADCx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if (pADCx == ADC1) RCC->APB2ENR |= (1 << 8);
        else if (pADCx == ADC2) RCC->APB2ENR |= (1 << 9);
        else if (pADCx == ADC3) RCC->APB2ENR |= (1 << 10);
    }
    else{
        if (pADCx == ADC1) RCC->APB2ENR &= ~(1 << 8);
        else if (pADCx == ADC2) RCC->APB2ENR &= ~(1 << 9);
        else if (pADCx == ADC3) RCC->APB2ENR &= ~(1 << 10);
    }
}

void ADC_Init(ADC_Handle_t* pADCHandle){
    ADC_CLK_Control(pADCHandle->pADCx, Enable);

    uint32_t temp = 0;

    // 1. Configure Resolution
    temp = (pADCHandle->ADC_Config.ADC_Resolution << 24);
    pADCHandle->pADCx->CR1 &= ~(0x03 << 24);
    pADCHandle->pADCx->CR1 |= temp;

    // 2. Configure Data Alignment
    temp = (pADCHandle->ADC_Config.ADC_DataAlign << 11);
    pADCHandle->pADCx->CR2 &= ~(1 << 11);
    pADCHandle->pADCx->CR2 |= temp;

    // 3. Configure Continuous Conversion Mode
    temp = (pADCHandle->ADC_Config.ADC_ContinuousConvMode << 1);
    pADCHandle->pADCx->CR2 &= ~(1 << 1);
    pADCHandle->pADCx->CR2 |= temp;

    // 4. Enable ADC via ADON bit
    pADCHandle->pADCx->CR2 |= (1 << 0);
}

void ADC_Reset(ADC_RegDef_t* pADCx){
    // ADCs share a reset bit on APB2
    RCC->APB2RSTR |= (1 << 8);
    RCC->APB2RSTR &= ~(1 << 8);
}

void ADC_StartConversion(ADC_RegDef_t* pADCx){
    // Start regular conversion (SWSTART)
    pADCx->CR2 |= (1 << 30);
}

uint16_t ADC_ReadValue(ADC_RegDef_t* pADCx){
    return (uint16_t)(pADCx->DR & 0xFFFF);
}

uint8_t ADC_GetEOCFlag(ADC_RegDef_t* pADCx){
    uint8_t result = 0;
    result = (0x01 & (pADCx->SR >> 1));
    return result;
}

void ADC_ClearEOCFlag(ADC_RegDef_t* pADCx){
    pADCx->SR &= ~(1 << 1);
}

void ADC_IRQInterrupt_Enable(ADC_RegDef_t* pADCx, uint8_t EnorDi){
    if(EnorDi == Enable){
        pADCx->CR1 |= (1 << 5); // EOCIE bit
    }
    else{
        pADCx->CR1 &= ~(1 << 5);
    }
}

void ADC_IRQInterrupt_Config(IRQn_Type IRQNumber, uint8_t EnorDi) {
    // Only access NVIC registers for positive IRQ numbers (STM32 Peripherals)
    if (IRQNumber >= 0) {
        uint8_t reg = IRQNumber / 32;
        uint8_t reg_place = IRQNumber % 32;
        
        if (EnorDi == Enable) { // Assuming 'Enable' is a macro you defined (e.g., 1)
            *(NVIC_ISER0 + reg) |= (1 << reg_place);
        } else {
            *(NVIC_ICER0 + reg) |= (1 << reg_place);
        }
    }
}

void ADC_IRQInterrupt_Priority(IRQn_Type IRQNumber, uint8_t IRQPriority) {
    if (IRQNumber >= 0) {
        uint8_t reg = IRQNumber / 4;
        uint8_t reg_place = IRQNumber % 4;
        
        *(NVIC_IPR0 + reg) &= ~(0xFF << (reg_place * 8));
        *(NVIC_IPR0 + reg) |= ((IRQPriority << 4) << (reg_place * 8));
        
    } else {
        uint8_t index = (IRQNumber & 0xF) - 4;
        
        SCB_SHPR[index] = (IRQPriority << 4);
    }
}