#include "Stm32f407g_disc1_spi.h"

void SPI_CLK_Control(SPI_RegDef_t* pSPIx, uint8_t EnorDi){
    if(EnorDi == Enable){
        if(pSPIx == SPI1) RCC->APB2ENR |= (1 << 12);
        else if(pSPIx == SPI2) RCC->APB1ENR |= (1 << 14);
        else if(pSPIx == SPI3) RCC->APB1ENR |= (1 << 15);
    } else {
        if(pSPIx == SPI1) RCC->APB2ENR &= ~(1 << 12);
        else if(pSPIx == SPI2) RCC->APB1ENR &= ~(1 << 14);
        else if(pSPIx == SPI3) RCC->APB1ENR &= ~(1 << 15);
    }
}

void SPI_Init(SPI_Handle_t* pSPIHandle){
    SPI_CLK_Control(pSPIHandle->pSPIx, Enable);
    uint32_t temp = 0;

    // 1. Mode
    temp |= (pSPIHandle->SPI_Config.SPI_DeviceMode << 2);

    // 2. Bus Config
    if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_FULL_DUPLEX){
        temp &= ~(1 << 15);
    } else if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_HALF_DUPLEX){
        temp |= (1 << 15);
    } else if(pSPIHandle->SPI_Config.SPI_BusConfig == SPI_BUS_SIMPLEX_RXONLY){
        temp &= ~(1 << 15);
        temp |= (1 << 10);
    }

    // 3. SCLK Speed
    temp |= (pSPIHandle->SPI_Config.SPI_SclkSpeed << 3);

    // 4. DFF, CPOL, CPHA, SSM
    temp |= (pSPIHandle->SPI_Config.SPI_DFF << 11);
    temp |= (pSPIHandle->SPI_Config.SPI_CPOL << 1);
    temp |= (pSPIHandle->SPI_Config.SPI_CPHA << 0);
    temp |= (pSPIHandle->SPI_Config.SPI_SSM << 9);

    pSPIHandle->pSPIx->CR1 = temp;
}

void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t EnorDi){
    if(EnorDi == Enable) pSPIx->CR1 |= (1 << 6);
    else pSPIx->CR1 &= ~(1 << 6);
}

void SPI_SSIConfig(SPI_RegDef_t* pSPIx, uint8_t EnorDi){
    if(EnorDi == Enable) pSPIx->CR1 |= (1 << 8);
    else pSPIx->CR1 &= ~(1 << 8);
}

void SPI_Tx(SPI_RegDef_t* pSPIx, uint8_t* pTxBuffer, uint32_t Len){
    while(Len > 0){
        while(!(pSPIx->SR & (1 << 1))); // Wait for TXE
        
        if((pSPIx->CR1 & (1 << 11))){ // 16-bit DFF
            pSPIx->DR = *((uint16_t*)pTxBuffer);
            Len -= 2;
            (uint16_t*)pTxBuffer++;
        } else { // 8-bit DFF
            pSPIx->DR = *pTxBuffer;
            Len--;
            pTxBuffer++;
        }
    }
}

void SPI_Rx(SPI_RegDef_t* pSPIx, uint8_t* pRxBuffer, uint32_t Len){
    while(Len > 0){
        while(!(pSPIx->SR & (1 << 0))); // Wait for RXNE
        
        if((pSPIx->CR1 & (1 << 11))){ 
            *((uint16_t*)pRxBuffer) = pSPIx->DR;
            Len -= 2;
            (uint16_t*)pRxBuffer++;
        } else {
            *pRxBuffer = pSPIx->DR;
            Len--;
            pRxBuffer++;
        }
    }
}