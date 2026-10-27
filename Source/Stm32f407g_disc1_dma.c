#include "Stm32f407g_disc1_dma.h"

void DMA_CLK_Control(DMA_RegDef_t* pDMAx, uint8_t EnorDi){
    if(EnorDi == Enable){
        if(pDMAx == DMA1) RCC->AHB1ENR |= (1 << 21);
        else if(pDMAx == DMA2) RCC->AHB1ENR |= (1 << 22);
    } else {
        if(pDMAx == DMA1) RCC->AHB1ENR &= ~(1 << 21);
        else if(pDMAx == DMA2) RCC->AHB1ENR &= ~(1 << 22);
    }
}

void DMA_Init(DMA_Handle_t* pDMAHandle){
    uint32_t temp = 0;
    
    // Disable stream before config
    pDMAHandle->pDMA_Stream->CR &= ~(1 << 0);
    while(pDMAHandle->pDMA_Stream->CR & (1 << 0));

    // 1. Channel selection
    temp |= (pDMAHandle->DMA_Config.DMA_Channel << 25);
    // 2. Direction
    temp |= (pDMAHandle->DMA_Config.DMA_Direction << 6);
    // 3. Memory & Peripheral Increment
    if(pDMAHandle->DMA_Config.DMA_MemInc) temp |= (1 << 10);
    if(pDMAHandle->DMA_Config.DMA_PeriphInc) temp |= (1 << 9);
    // 4. Data sizes
    temp |= (pDMAHandle->DMA_Config.DMA_MemDataSize << 13);
    temp |= (pDMAHandle->DMA_Config.DMA_PeriphDataSize << 11);
    // 5. Circular Mode
    if(pDMAHandle->DMA_Config.DMA_Mode) temp |= (1 << 8);

    pDMAHandle->pDMA_Stream->CR = temp;
}

void DMA_StartTx(DMA_Handle_t* pDMAHandle, uint32_t SrcAddr, uint32_t DestAddr, uint32_t Len){
    if(pDMAHandle->DMA_Config.DMA_Direction == DMA_DIR_M2P){
        pDMAHandle->pDMA_Stream->M0AR = SrcAddr;
        pDMAHandle->pDMA_Stream->PAR = DestAddr;
    } else {
        pDMAHandle->pDMA_Stream->PAR = SrcAddr;
        pDMAHandle->pDMA_Stream->M0AR = DestAddr;
    }
    
    pDMAHandle->pDMA_Stream->NDTR = Len;
    pDMAHandle->pDMA_Stream->CR |= (1 << 0); // Enable stream
}