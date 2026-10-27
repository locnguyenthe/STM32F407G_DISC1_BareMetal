#ifndef _STM32F407G_DISC1_DMA_H_
#define _STM32F407G_DISC1_DMA_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

// DMA Stream definition
typedef struct{
    __vo uint32_t CR;         // 0x00
    __vo uint32_t NDTR;       // 0x04
    __vo uint32_t PAR;        // 0x08
    __vo uint32_t M0AR;       // 0x0C
    __vo uint32_t M1AR;       // 0x10
    __vo uint32_t FCR;        // 0x14
}DMA_Stream_RegDef_t;

// DMA controller definition
typedef struct{
    __vo uint32_t LISR;       // 0x00
    __vo uint32_t HISR;       // 0x04
    __vo uint32_t LIFCR;      // 0x08
    __vo uint32_t HIFCR;      // 0x0C
    DMA_Stream_RegDef_t S[8]; // Streams 0 to 7
}DMA_RegDef_t;

#define DMA1_Base                   0x40026000u
#define DMA2_Base                   0x40026400u

#define DMA1                        ((DMA_RegDef_t*)DMA1_Base)
#define DMA2                        ((DMA_RegDef_t*)DMA2_Base)

typedef struct{
    uint8_t  DMA_Channel;
    uint8_t  DMA_Direction;
    uint8_t  DMA_MemInc;
    uint8_t  DMA_PeriphInc;
    uint8_t  DMA_MemDataSize;
    uint8_t  DMA_PeriphDataSize;
    uint8_t  DMA_Mode;
}DMA_Config_t;

typedef struct{
    DMA_Stream_RegDef_t* pDMA_Stream;
    DMA_Config_t         DMA_Config;
}DMA_Handle_t;

// DMA Direction
#define DMA_DIR_P2M                 0
#define DMA_DIR_M2P                 1
#define DMA_DIR_M2M                 2

// Data Size
#define DMA_DSIZE_BYTE              0
#define DMA_DSIZE_HALFWORD          1
#define DMA_DSIZE_WORD              2

void DMA_CLK_Control(DMA_RegDef_t* pDMAx, uint8_t EnorDi);
void DMA_Init(DMA_Handle_t* pDMAHandle);
void DMA_StartTx(DMA_Handle_t* pDMAHandle, uint32_t SrcAddr, uint32_t DestAddr, uint32_t Len);

#endif