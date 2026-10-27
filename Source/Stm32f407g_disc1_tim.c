#include <stdint.h>
#include "Stm32f407g_disc1_tim.h"

void TIM_CLK_Control(TIM_RegDef_t* pTIMx, uint8_t EnorDi){
    // TIM2 to TIM7 are mapped sequentially on APB1 starting at 0x40000000 with 0x400 offset
    uint8_t timCode = ((uint32_t)pTIMx - TIM2_Base) / 0x400;
    if (EnorDi == Enable){
        RCC->APB1ENR |= (1 << timCode);
    }
    else{
        RCC->APB1ENR &= ~(1 << timCode);
    }
}

void TIM_Init(TIM_Handle_t* pTIMHandle){
    TIM_CLK_Control(pTIMHandle->pTIMx, Enable);

    uint32_t temp = 0;

    // 1. Configure Counter Mode (DIR bit 4, CMS bits 6:5 in CR1)
    if (pTIMHandle->TIM_Config.TIM_CounterMode == TIM_COUNTER_UP){
        pTIMHandle->pTIMx->CR1 &= ~(0x07 << 4);
    }
    else if (pTIMHandle->TIM_Config.TIM_CounterMode == TIM_COUNTER_DOWN){
        temp = (1 << 4);
        pTIMHandle->pTIMx->CR1 &= ~(0x07 << 4);
        pTIMHandle->pTIMx->CR1 |= temp;
    }
    else if (pTIMHandle->TIM_Config.TIM_CounterMode == TIM_COUNTER_CENTER_ALIGNED1){
        temp = (1 << 5);
        pTIMHandle->pTIMx->CR1 &= ~(0x07 << 4);
        pTIMHandle->pTIMx->CR1 |= temp;
    }
    else if (pTIMHandle->TIM_Config.TIM_CounterMode == TIM_COUNTER_CENTER_ALIGNED2){
        temp = (2 << 5);
        pTIMHandle->pTIMx->CR1 &= ~(0x07 << 4);
        pTIMHandle->pTIMx->CR1 |= temp;
    }
    else if (pTIMHandle->TIM_Config.TIM_CounterMode == TIM_COUNTER_CENTER_ALIGNED3){
        temp = (3 << 5);
        pTIMHandle->pTIMx->CR1 &= ~(0x07 << 4);
        pTIMHandle->pTIMx->CR1 |= temp;
    }

    // 2. Configure Auto-Reload Preload (ARPE bit 7 in CR1)
    temp = 0;
    if (pTIMHandle->TIM_Config.TIM_AutoReloadPreload == TIM_ARP_DISABLE){
        pTIMHandle->pTIMx->CR1 &= ~(1 << 7);
    }
    else if (pTIMHandle->TIM_Config.TIM_AutoReloadPreload == TIM_ARP_ENABLE){
        temp = (1 << 7);
        pTIMHandle->pTIMx->CR1 &= ~(1 << 7);
        pTIMHandle->pTIMx->CR1 |= temp;
    }

    // 3. Configure Clock Source (Internal vs External Mode 1 vs External Mode 2 in SMCR)
    temp = 0;
    if (pTIMHandle->TIM_Config.TIM_ClockSource == TIM_CLK_INTERNAL){
        pTIMHandle->pTIMx->SMCR &= ~(0x07 << 0); // SMS = 000
        pTIMHandle->pTIMx->SMCR &= ~(1 << 14);   // ECE = 0
    }
    else if (pTIMHandle->TIM_Config.TIM_ClockSource == TIM_CLK_EXT_MODE1){
        // Set Trigger Selection (TS bits 6:4) and Slave Mode Selection (SMS = 111 at bits 2:0)
        temp = ((pTIMHandle->TIM_Config.TIM_ExtTriggerSource & 0x07) << 4) | (0x07 << 0);   //Set the bits of TIM_ExtTriggerSource0111
        pTIMHandle->pTIMx->SMCR &= ~(((0x07 << 4) | (0x07 << 0)));                          //Clear the TS (6:4) and SMS (2:0) of SMCR reg
        pTIMHandle->pTIMx->SMCR |= temp;                                                    //So the SMS (2:0) is hardcoded 111 and TS (6:4) is set by TIM_ExtTriggerSource
    }
    else if (pTIMHandle->TIM_Config.TIM_ClockSource == TIM_CLK_EXT_MODE2){
        // Set External Trigger Polarity (ETP bit 15) and External Clock Enable (ECE bit 14)
        temp = ((pTIMHandle->TIM_Config.TIM_ExtTriggerPolarity & 0x01) << 15) | (1 << 14);
        pTIMHandle->pTIMx->SMCR &= ~((1 << 15) | (1 << 14));
        pTIMHandle->pTIMx->SMCR |= temp;
    }

    // 4. Configure Prescaler and Auto-Reload Register
    pTIMHandle->pTIMx->PSC = pTIMHandle->TIM_Config.TIM_Prescaler;
    pTIMHandle->pTIMx->ARR = pTIMHandle->TIM_Config.TIM_Period;

    // 5. Generate an Update Event to load the Prescaler value immediately, then clear UIF
    pTIMHandle->pTIMx->EGR |= (1 << 0);
    pTIMHandle->pTIMx->SR &= ~(1 << 0);
}

void TIM_Reset(TIM_RegDef_t* pTIMx){
    uint8_t timCode = ((uint32_t)pTIMx - TIM2_Base) / 0x400;
    RCC->APB1RSTR |= (1 << timCode);
    RCC->APB1RSTR &= ~(1 << timCode);
}

void TIM_Start(TIM_RegDef_t* pTIMx, uint8_t EnorDi){
    if (EnorDi == Enable){
        pTIMx->CR1 |= (1 << 0);
    }
    else{
        pTIMx->CR1 &= ~(1 << 0);
    }
}

uint32_t TIM_ReadCounter(TIM_RegDef_t* pTIMx){
    return pTIMx->CNT;
}

void TIM_WriteCounter(TIM_RegDef_t* pTIMx, uint32_t value){
    pTIMx->CNT = value;
}

uint8_t TIM_GetUpdateFlag(TIM_RegDef_t* pTIMx){
    uint8_t result = 0;
    result = (0x01 & (pTIMx->SR >> 0));
    return result;
}

void TIM_ClearUpdateFlag(TIM_RegDef_t* pTIMx){
    pTIMx->SR &= ~(1 << 0);
}

void TIM_IRQInterrupt_Enable(TIM_RegDef_t* pTIMx, uint8_t EnorDi){
    if (EnorDi == Enable){
        pTIMx->DIER |= (1 << 0); // UIE: Update interrupt enable
    }
    else{
        pTIMx->DIER &= ~(1 << 0);
    }
}

void TIM_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
    uint8_t reg = 0;
    uint8_t reg_place = 0;
    reg = IRQNumber / 32;
    reg_place = IRQNumber - (reg * 32);
    if (EnorDi == Enable){
        *(NVIC_ISER0 + reg) |= (1 << reg_place);
    }
    else{
        *(NVIC_ICER0 + reg) |= (1 << reg_place);
    }
}

void TIM_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = 0;
    uint8_t reg_place = 0;
    reg = IRQNumber / 4;
    reg_place = IRQNumber - (reg * 4);
    *(NVIC_IPR0 + reg) &= ~(0xFF << (reg_place * 8));
    *(NVIC_IPR0 + reg) |= ((IRQPriority << 4) << (reg_place * 8));
}

void TIM_IRQInterrupt_Handling(TIM_RegDef_t* pTIMx){
    if (pTIMx->SR & (1 << 0)){
        pTIMx->SR &= ~(1 << 0); // Clear UIF flag
    }
}