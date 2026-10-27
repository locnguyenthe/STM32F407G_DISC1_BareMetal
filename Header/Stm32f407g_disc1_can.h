#ifndef _STM32F407G_DISC1_CAN_H_
#define _STM32F407G_DISC1_CAN_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

typedef struct{
    __vo uint32_t MCR;        // 0x00
    __vo uint32_t MSR;        // 0x04
    __vo uint32_t TSR;        // 0x08
    __vo uint32_t RF0R;       // 0x0C
    __vo uint32_t RF1R;       // 0x10
    __vo uint32_t IER;        // 0x14
    __vo uint32_t ESR;        // 0x18
    __vo uint32_t BTR;        // 0x1C
    uint32_t      Reserved[88]; // 0x20 to 0x17C padding
    // TX Mailboxes (0x180 - 0x1AC)
    __vo uint32_t TI0R;       
    __vo uint32_t TDT0R;
    __vo uint32_t TDL0R;
    __vo uint32_t TDH0R;
    __vo uint32_t TI1R;
    __vo uint32_t TDT1R;
    __vo uint32_t TDL1R;
    __vo uint32_t TDH1R;
    __vo uint32_t TI2R;
    __vo uint32_t TDT2R;
    __vo uint32_t TDL2R;
    __vo uint32_t TDH2R;
    // Rx FIFOs (0x1B0 - 0x1CC)
    __vo uint32_t RI0R;
    __vo uint32_t RDT0R;
    __vo uint32_t RDL0R;
    __vo uint32_t RDH0R;
    __vo uint32_t RI1R;
    __vo uint32_t RDT1R;
    __vo uint32_t RDL1R;
    __vo uint32_t RDH1R;
    uint32_t      Reserved1[12]; // 0x1D0 to 0x1FC
    // Filter Registers (0x200)
    __vo uint32_t FMR;
    __vo uint32_t FM1R;
    __vo uint32_t FS1R;
    __vo uint32_t FFA1R;
    __vo uint32_t FA1R;
}CAN_RegDef_t;

#define CAN1_Base                   0x40006400u
#define CAN2_Base                   0x40006800u

#define CAN1                        ((CAN_RegDef_t*)CAN1_Base)
#define CAN2                        ((CAN_RegDef_t*)CAN2_Base)

typedef struct{
    uint8_t  CAN_Mode;
    uint8_t  CAN_SJW;
    uint8_t  CAN_BS1;
    uint8_t  CAN_BS2;
    uint16_t CAN_Prescaler;
}CAN_Config_t;

typedef struct{
    CAN_RegDef_t* pCANx;
    CAN_Config_t  CAN_Config;
}CAN_Handle_t;

// CAN Modes
#define CAN_MODE_NORMAL             0
#define CAN_MODE_LOOPBACK           1
#define CAN_MODE_SILENT             2
#define CAN_MODE_SILENT_LOOPBACK    3

void CAN_CLK_Control(CAN_RegDef_t* pCANx, uint8_t EnorDi);
void CAN_Init(CAN_Handle_t* pCANHandle);

#endif