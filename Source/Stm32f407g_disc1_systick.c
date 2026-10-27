#include <stdint.h>
#include "Stm32f407g_disc1_systick.h"

void SYSTICK_Init(SYSTICK_Handle_t* pSYSTICKHandle){
    uint32_t temp = 0;

    // 1. Disable SysTick before configuration
    pSYSTICKHandle->pSYSTICK->CSR &= ~(1 << 0);

    // 2. Configure Reload Value (24-bit max: 0x00FFFFFF)
    pSYSTICKHandle->pSYSTICK->RVR = (pSYSTICKHandle->SYSTICK_Config.SYSTICK_ReloadValue & 0x00FFFFFFu);

    // 3. Clear Current Value Register
    pSYSTICKHandle->pSYSTICK->CVR = 0;

    // 4. Configure Clock Source (Bit 2: CLKSOURCE)
    if (pSYSTICKHandle->SYSTICK_Config.SYSTICK_ClockSource == SYSTICK_CLK_AHB_DIV8){
        pSYSTICKHandle->pSYSTICK->CSR &= ~(1 << 2);
    }
    else if (pSYSTICKHandle->SYSTICK_Config.SYSTICK_ClockSource == SYSTICK_CLK_AHB){
        pSYSTICKHandle->pSYSTICK->CSR |= (1 << 2);
    }

    // 5. Configure SysTick Interrupt (Bit 1: TICKINT)
    if (pSYSTICKHandle->SYSTICK_Config.SYSTICK_Interrupt == SYSTICK_INT_DISABLE){
        pSYSTICKHandle->pSYSTICK->CSR &= ~(1 << 1);
    }
    else if (pSYSTICKHandle->SYSTICK_Config.SYSTICK_Interrupt == SYSTICK_INT_ENABLE){
        pSYSTICKHandle->pSYSTICK->CSR |= (1 << 1);
    }
}

void SYSTICK_Start(uint8_t EnorDi){
    if (EnorDi == Enable){
        SYSTICK->CSR |= (1 << 0);
    }
    else{
        SYSTICK->CSR &= ~(1 << 0);
    }
}

uint8_t SYSTICK_GetCountFlag(void){
    uint8_t result = 0;
    result = (0x01 & (SYSTICK->CSR >> 16));
    return result;
}

uint32_t SYSTICK_ReadCurrentValue(void){
    return (SYSTICK->CVR & 0x00FFFFFFu);
}

void SYSTICK_DelayMs(uint32_t ms){
    // Configure for 1 ms tick using processor clock (16,000,000 / 1,000 - 1 = 15,999)
    SYSTICK->CSR &= ~(1 << 0);
    SYSTICK->RVR = (SYSTEM_CORE_CLOCK / 1000u) - 1u;
    SYSTICK->CVR = 0;
    SYSTICK->CSR |= (1 << 2); // Processor clock (AHB)
    SYSTICK->CSR |= (1 << 0); // Enable counter

    for (uint32_t i = 0; i < ms; i++){
        while (SYSTICK_GetCountFlag() == Reset);
    }

    SYSTICK->CSR &= ~(1 << 0); // Disable counter after delay
}

void SYSTICK_DelayUs(uint32_t us){
    // Configure for 1 us tick using processor clock (16,000,000 / 1,000,000 - 1 = 15)
    SYSTICK->CSR &= ~(1 << 0);
    SYSTICK->RVR = (SYSTEM_CORE_CLOCK / 1000000u) - 1u;
    SYSTICK->CVR = 0;
    SYSTICK->CSR |= (1 << 2); // Processor clock (AHB)
    SYSTICK->CSR |= (1 << 0); // Enable counter

    for (uint32_t i = 0; i < us; i++){
        while (SYSTICK_GetCountFlag() == Reset);
    }

    SYSTICK->CSR &= ~(1 << 0); // Disable counter after delay
}

void SYSTICK_IRQInterrupt_Config(uint8_t EnorDi){
    if (EnorDi == Enable){
        SYSTICK->CSR |= (1 << 1);
    }
    else{
        SYSTICK->CSR &= ~(1 << 1);
    }
}