#include "Stm32f407g_disc1_can.h"

void CAN_CLK_Control(CAN_RegDef_t* pCANx, uint8_t EnorDi){
    if(EnorDi == Enable){
        if(pCANx == CAN1) RCC->APB1ENR |= (1 << 25);
        else if(pCANx == CAN2) RCC->APB1ENR |= (1 << 26);
    } else {
        if(pCANx == CAN1) RCC->APB1ENR &= ~(1 << 25);
        else if(pCANx == CAN2) RCC->APB1ENR &= ~(1 << 26);
    }
}

void CAN_Init(CAN_Handle_t* pCANHandle){
    CAN_CLK_Control(pCANHandle->pCANx, Enable);

    // 1. Enter Initialization Mode
    pCANHandle->pCANx->MCR &= ~(1 << 1); // Exit Sleep
    pCANHandle->pCANx->MCR |= (1 << 0);  // INRQ Request
    while(!(pCANHandle->pCANx->MSR & (1 << 0))); // Wait for INAK

    // 2. Setup Bit Timing & Mode
    uint32_t btr = 0;
    
    // Set Mode (LBKM and SILM in BTR)
    if(pCANHandle->CAN_Config.CAN_Mode == CAN_MODE_LOOPBACK) btr |= (1 << 30);
    else if(pCANHandle->CAN_Config.CAN_Mode == CAN_MODE_SILENT) btr |= (1 << 31);
    else if(pCANHandle->CAN_Config.CAN_Mode == CAN_MODE_SILENT_LOOPBACK) btr |= (3 << 30);

    // Set Resynchronization Jump Width, Time Segments, and Prescaler
    btr |= ((pCANHandle->CAN_Config.CAN_SJW & 0x03) << 24);
    btr |= ((pCANHandle->CAN_Config.CAN_BS2 & 0x07) << 20);
    btr |= ((pCANHandle->CAN_Config.CAN_BS1 & 0x0F) << 16);
    btr |= ((pCANHandle->CAN_Config.CAN_Prescaler - 1) & 0x3FF);
    
    pCANHandle->pCANx->BTR = btr;

    // 3. Leave Initialization Mode
    pCANHandle->pCANx->MCR &= ~(1 << 0);
    while((pCANHandle->pCANx->MSR & (1 << 0))); // Wait until INAK cleared
}