#include <stdint.h>
#include "Stm32f407g_disc1_usart.h"
#include "Stm32f407g_disc1_rcc.h"

void USART_CLK_Control(USART_RegDef_t* pUSARTx, uint8_t EnorDi){
    if (EnorDi == Enable){
        if(pUSARTx == USART1) RCC->APB2ENR |= (1 << 4);
        else if(pUSARTx == USART2) RCC->APB1ENR |= (1 << 17);
        else if(pUSARTx == USART3) RCC->APB1ENR |= (1 << 18);
        else if(pUSARTx == UART4)  RCC->APB1ENR |= (1 << 19);
        else if(pUSARTx == UART5)  RCC->APB1ENR |= (1 << 20);
        else if(pUSARTx == USART6) RCC->APB2ENR |= (1 << 5);
    }
    else{
        if(pUSARTx == USART1) RCC->APB2ENR &= ~(1 << 4);
        else if(pUSARTx == USART2) RCC->APB1ENR &= ~(1 << 17);
        else if(pUSARTx == USART3) RCC->APB1ENR &= ~(1 << 18);
        else if(pUSARTx == UART4)  RCC->APB1ENR &= ~(1 << 19);
        else if(pUSARTx == UART5)  RCC->APB1ENR &= ~(1 << 20);
        else if(pUSARTx == USART6) RCC->APB2ENR &= ~(1 << 5);
    }
}

void USART_Init(USART_Handle_t* pUSARTHandle){
    USART_CLK_Control(pUSARTHandle->pUSARTx, Enable);

    uint32_t temp = 0;

    // 1. Configure Mode (TE/RE bits in CR1)
    if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_RX){
        temp |= (1 << 2);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_ONLY_TX){
        temp |= (1 << 3);
    }
    else if (pUSARTHandle->USART_Config.USART_Mode == USART_MODE_TXRX){
        temp |= ((1 << 2) | (1 << 3));
    }
    pUSARTHandle->pUSARTx->CR1 &= ~((1 << 2) | (1 << 3));
    pUSARTHandle->pUSARTx->CR1 |= temp;

    // 2. Configure Word Length (M bit in CR1)
    temp = (pUSARTHandle->USART_Config.USART_WordLength << 12);
    pUSARTHandle->pUSARTx->CR1 &= ~(1 << 12);
    pUSARTHandle->pUSARTx->CR1 |= temp;

    // 3. Configure Parity (PCE & PS bits in CR1)
    if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_EVEN){
        temp = (1 << 10);
        temp |= (0 << 9);
    }
    else if (pUSARTHandle->USART_Config.USART_ParityControl == USART_PARITY_EN_ODD){
        temp = (1 << 10);
        temp |= (1 << 9);
    }
    else {
        temp = 0;
    }
    pUSARTHandle->pUSARTx->CR1 &= ~((1 << 10) | (1 << 9));
    pUSARTHandle->pUSARTx->CR1 |= temp;

    // 4. Configure Stop Bits (STOP bits in CR2)
    temp = (pUSARTHandle->USART_Config.USART_NoOfStopBits << 12);
    pUSARTHandle->pUSARTx->CR2 &= ~(0x03 << 12);
    pUSARTHandle->pUSARTx->CR2 |= temp;

    // 5. Configure Hardware Flow Control (CTSE & RTSE bits in CR3)
    if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS){
        temp = (1 << 9);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS){
        temp = (1 << 8);
    }
    else if (pUSARTHandle->USART_Config.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS){
        temp = (1 << 8) | (1 << 9);
    }
    else {
        temp = 0;
    }
    pUSARTHandle->pUSARTx->CR3 &= ~((1 << 8) | (1 << 9));
    pUSARTHandle->pUSARTx->CR3 |= temp;

    // 6. Configure Baud Rate
    uint32_t pclk;
    if(pUSARTHandle->pUSARTx == USART1 || pUSARTHandle->pUSARTx == USART6){
        pclk = RCC_GetPCLK2Value();  // APB2 Peripherals
    } else {
        pclk = RCC_GetPCLK1Value();  // APB1 Peripherals
    }

    uint32_t usartdiv = (pclk * 25) / (4 * pUSARTHandle->USART_Config.USART_Baud);
    uint32_t mantissa = usartdiv / 100;
    uint32_t fraction = usartdiv - (mantissa * 100);
    
    // For 16x oversampling calculation
    fraction = (((fraction * 16) + 50) / 100) & 0x0F;
    pUSARTHandle->pUSARTx->BRR = (mantissa << 4) | fraction;

    // 7. Enable USART (UE bit in CR1)
    pUSARTHandle->pUSARTx->CR1 |= (1 << 13);
}

void USART_Reset(USART_RegDef_t* pUSARTx){
    if(pUSARTx == USART1)      { RCC->APB2RSTR |= (1 << 4);  RCC->APB2RSTR &= ~(1 << 4); }
    else if(pUSARTx == USART2) { RCC->APB1RSTR |= (1 << 17); RCC->APB1RSTR &= ~(1 << 17); }
    else if(pUSARTx == USART3) { RCC->APB1RSTR |= (1 << 18); RCC->APB1RSTR &= ~(1 << 18); }
    else if(pUSARTx == UART4)  { RCC->APB1RSTR |= (1 << 19); RCC->APB1RSTR &= ~(1 << 19); }
    else if(pUSARTx == UART5)  { RCC->APB1RSTR |= (1 << 20); RCC->APB1RSTR &= ~(1 << 20); }
    else if(pUSARTx == USART6) { RCC->APB2RSTR |= (1 << 5);  RCC->APB2RSTR &= ~(1 << 5); }
}

void USART_Tx(USART_RegDef_t* pUSARTx, uint8_t* pTxBuffer, uint32_t Len){
    for (uint32_t i = 0; i < Len; i++){
        // Wait until TXE (Transmit data register empty) flag is set
        while (!(pUSARTx->SR & (1 << 7)));

        pUSARTx->DR = (*pTxBuffer & (uint16_t)0xFF);
        pTxBuffer++;
    }
    // Wait until TC (Transmission complete) flag is set
    while (!(pUSARTx->SR & (1 << 6)));
}

void USART_Rx(USART_RegDef_t* pUSARTx, uint8_t* pRxBuffer, uint32_t Len){
    for (uint32_t i = 0; i < Len; i++){
        // Wait until RXNE (Read data register not empty) flag is set
        while (!(pUSARTx->SR & (1 << 5)));

        *pRxBuffer = (pUSARTx->DR & (uint8_t)0xFF);
        pRxBuffer++;
    }
}

void USART_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi){
    uint8_t reg = IRQNumber / 32;
    uint8_t reg_place = IRQNumber % 32;
    if (EnorDi == Enable){
        *(NVIC_ISER0 + reg) |= (1 << reg_place);
    }
    else{
        *(NVIC_ICER0 + reg) |= (1 << reg_place);
    }
}

void USART_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority){
    uint8_t reg = IRQNumber / 4;
    uint8_t reg_place = IRQNumber % 4;
    *(NVIC_IPR0 + reg) &= ~(0xFF << (reg_place * 8));
    *(NVIC_IPR0 + reg) |= ((IRQPriority << 4) << (reg_place * 8));
}