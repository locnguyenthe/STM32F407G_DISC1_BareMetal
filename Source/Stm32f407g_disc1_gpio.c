#include <stdint.h>
#include "Stm32f407g_disc1_gpio.h"

void GPIO_CLK_Control(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi){
    uint8_t portCode = ((uint32_t)pGPIOx - GPIOA_Base) / 0x400;
    if(EnorDi == Enable){
        RCC->AHB1ENR |= (1 << portCode);
    }
    else{
        RCC->AHB1ENR &= ~(1 << portCode);
    }
}

void GPIO_Init(GPIO_Handle_t* pGPIOHandle){
    GPIO_CLK_Control(pGPIOHandle->pGPIOx, Enable);

    uint32_t temp = 0;
    // Configure mode
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->MODER &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->MODER |= temp;
    }
    else{
        // 1. Configure the EXTI Edge Trigger
        if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT) {
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        } 
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT) {
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        } 
        else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT_RT) {
            EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
            EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        }

        // 2. Enable the SYSCFG Clock
        RCC->APB2ENR |= (1 << 14);

        // 3. Route the GPIO port to the correct EXTI line in SYSCFG_EXTICR
        uint8_t exticr_reg = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
        uint8_t exticr_pos = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4) * 4;
        uint8_t portCode = ((uint32_t)pGPIOHandle->pGPIOx - GPIOA_Base) / 0x400;

        // Clear the specific 4 bits, then set them to the port code (0=PA, 1=PB, etc.)
        SYSCFG->EXTICR[exticr_reg] &= ~(0x0F << exticr_pos);
        SYSCFG->EXTICR[exticr_reg] |= (portCode << exticr_pos);

        // 4. Unmask the EXTI line
        EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    }
    //Configure Speed
    temp = 0;
    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed == GPIO_Speed_LOW){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    }
    else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed == GPIO_Speed_MEDIUM){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    }
    else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed == GPIO_Speed_FAST){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    }
    else if ((pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed == GPIO_Speed_HIGH)){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->OSPEEDR |= temp;
    }

    //Pull-up and Pull-down
    temp = 0;
    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD == GPIO_NO_Pullup_Pulldown){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->PUPDR |= temp;
    }
    else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD == GPIO_Pullup){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->PUPDR |= temp;
    }
    else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD == GPIO_Pulldown){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->PUPDR &= ~(0x03 << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber * 2));
        pGPIOHandle->pGPIOx->PUPDR |= temp;
    }

    //Output Type
    temp = 0;
    if (pGPIOHandle->GPIO_PinConfig.GPIO_OutputType == GPIO_PushPull){
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_OutputType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        pGPIOHandle->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        pGPIOHandle->pGPIOx->OTYPER |= temp;
    }
    else{
        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_OutputType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        pGPIOHandle->pGPIOx->OTYPER &= ~(0x01 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
        pGPIOHandle->pGPIOx->OTYPER |= temp;
    }
    //Alternative Function
}

void GPIO_Reset(GPIO_RegDef_t* pGPIOx){
    uint8_t portCode = ((uint32_t)pGPIOx - GPIOA_Base) / 0x400;
    RCC->AHB1RSTR |= (1 << portCode);
    RCC->AHB1RSTR &= ~(1 << portCode);
}

uint8_t GPIO_Read(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber){
    uint8_t result = 0;
    result = (0x01 & (pGPIOx->IDR >> PinNumber));
    return result;
}

void GPIO_Write(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t value){
    uint16_t temp = 0;
    temp = ((value ? 1 : 0) << PinNumber);
    pGPIOx->ODR &= ~(1 << PinNumber);
    pGPIOx->ODR |= temp;
}

void GPIO_WriteToggle(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber){
    if(GPIO_Read(pGPIOx, PinNumber) == Set){
        GPIO_Write(pGPIOx, PinNumber, Reset);
    }
    else{
        GPIO_Write(pGPIOx, PinNumber, Set);
    }
}

void GPIO_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
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

void GPIO_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = 0;
    uint8_t reg_place = 0;
    reg = IRQNumber / 4;
    reg_place = IRQNumber - (reg * 4);
    *(NVIC_IPR0 + reg) &= ~(0xFF << (reg_place * 8));
    *(NVIC_IPR0 + reg) |= ((IRQPriority << 4) << (reg_place * 8));
}

void GPIO_IRQInterrupt_Handling(uint8_t PinNumber){
    if (EXTI->PR & (1 << PinNumber)){
        EXTI->PR = (1 << PinNumber);
    }
}