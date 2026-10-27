#ifndef _STM32F407G_DISC1_SPI_H_
#define _STM32F407G_DISC1_SPI_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

typedef struct{
    __vo uint32_t CR1;        // 0x00
    __vo uint32_t CR2;        // 0x04
    __vo uint32_t SR;         // 0x08
    __vo uint32_t DR;         // 0x0C
    __vo uint32_t CRCPR;      // 0x10
    __vo uint32_t RXCRCR;     // 0x14
    __vo uint32_t TXCRCR;     // 0x18
    __vo uint32_t I2SCFGR;    // 0x1C
    __vo uint32_t I2SPR;      // 0x20
}SPI_RegDef_t;

#define SPI1_Base                   0x40013000u
#define SPI2_Base                   0x40003800u
#define SPI3_Base                   0x40003C00u

#define SPI1                        ((SPI_RegDef_t*)SPI1_Base)
#define SPI2                        ((SPI_RegDef_t*)SPI2_Base)
#define SPI3                        ((SPI_RegDef_t*)SPI3_Base)

typedef struct{
    uint8_t SPI_DeviceMode;
    uint8_t SPI_BusConfig;
    uint8_t SPI_SclkSpeed;
    uint8_t SPI_DFF;
    uint8_t SPI_CPOL;
    uint8_t SPI_CPHA;
    uint8_t SPI_SSM;
}SPI_Config_t;

typedef struct{
    SPI_RegDef_t* pSPIx;
    SPI_Config_t  SPI_Config;
}SPI_Handle_t;

// SPI_DeviceMode
#define SPI_MODE_SLAVE              0
#define SPI_MODE_MASTER             1

// SPI_BusConfig
#define SPI_BUS_FULL_DUPLEX         1
#define SPI_BUS_HALF_DUPLEX         2
#define SPI_BUS_SIMPLEX_RXONLY      3

// SPI_SclkSpeed (Baud rate control)
#define SPI_SCLK_SPEED_DIV2         0
#define SPI_SCLK_SPEED_DIV4         1
#define SPI_SCLK_SPEED_DIV8         2
#define SPI_SCLK_SPEED_DIV16        3
#define SPI_SCLK_SPEED_DIV32        4
#define SPI_SCLK_SPEED_DIV64        5
#define SPI_SCLK_SPEED_DIV128       6
#define SPI_SCLK_SPEED_DIV256       7

// SPI_DFF (Data frame format)
#define SPI_DFF_8BITS               0
#define SPI_DFF_16BITS              1

// SPI_CPOL & SPI_CPHA
#define SPI_CPOL_LOW                0
#define SPI_CPOL_HIGH               1
#define SPI_CPHA_LOW                0
#define SPI_CPHA_HIGH               1

// SPI_SSM (Software slave management)
#define SPI_SSM_DISABLE             0
#define SPI_SSM_ENABLE              1

void SPI_CLK_Control(SPI_RegDef_t* pSPIx, uint8_t EnorDi);
void SPI_Init(SPI_Handle_t* pSPIHandle);
void SPI_PeripheralControl(SPI_RegDef_t* pSPIx, uint8_t EnorDi);
void SPI_SSIConfig(SPI_RegDef_t* pSPIx, uint8_t EnorDi);

void SPI_Tx(SPI_RegDef_t* pSPIx, uint8_t* pTxBuffer, uint32_t Len);
void SPI_Rx(SPI_RegDef_t* pSPIx, uint8_t* pRxBuffer, uint32_t Len);

#endif