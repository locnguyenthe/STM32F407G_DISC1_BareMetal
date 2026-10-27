#ifndef _STM32F407G_DISC1_USART_H_
#define _STM32F407G_DISC1_USART_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

// ================= REGISTER DEFINITIONS (Added here for modularity) =================
typedef struct{
    __vo uint32_t SR;               // 0x00 Status register
    __vo uint32_t DR;               // 0x04 Data register
    __vo uint32_t BRR;              // 0x08 Baud rate register
    __vo uint32_t CR1;              // 0x0C Control register 1
    __vo uint32_t CR2;              // 0x10 Control register 2
    __vo uint32_t CR3;              // 0x14 Control register 3
    __vo uint32_t GTPR;             // 0x18 Guard time and prescaler register
}USART_RegDef_t;

// USART Base Addresses
#define USART1_Base                 0x40011000u // APB2
#define USART2_Base                 0x40004400u // APB1
#define USART3_Base                 0x40004800u // APB1
#define UART4_Base                  0x40004C00u // APB1
#define UART5_Base                  0x40005000u // APB1
#define USART6_Base                 0x40011400u // APB2

#define USART1                      ((USART_RegDef_t*)USART1_Base)
#define USART2                      ((USART_RegDef_t*)USART2_Base)
#define USART3                      ((USART_RegDef_t*)USART3_Base)
#define UART4                       ((USART_RegDef_t*)UART4_Base)
#define UART5                       ((USART_RegDef_t*)UART5_Base)
#define USART6                      ((USART_RegDef_t*)USART6_Base)

typedef struct{
    uint8_t  USART_Mode;
    uint32_t USART_Baud;
    uint8_t  USART_NoOfStopBits;
    uint8_t  USART_WordLength;
    uint8_t  USART_ParityControl;
    uint8_t  USART_HWFlowControl;
}USART_Config_t;

typedef struct{
    USART_RegDef_t* pUSARTx;
    USART_Config_t  USART_Config;
}USART_Handle_t;

//USART_Mode
#define USART_MODE_ONLY_TX          0
#define USART_MODE_ONLY_RX          1
#define USART_MODE_TXRX             2

//USART_Baud
#define USART_STD_BAUD_1200         1200
#define USART_STD_BAUD_2400         2400
#define USART_STD_BAUD_9600         9600
#define USART_STD_BAUD_19200        19200
#define USART_STD_BAUD_38400        38400
#define USART_STD_BAUD_57600        57600
#define USART_STD_BAUD_115200       115200

//USART_NoOfStopBits
#define USART_STOPBITS_1            0
#define USART_STOPBITS_0_5          1
#define USART_STOPBITS_2            2
#define USART_STOPBITS_1_5          3

//USART_WordLength
#define USART_WORDLEN_8BITS         0
#define USART_WORDLEN_9BITS         1

//USART_ParityControl
#define USART_PARITY_DISABLE        0
#define USART_PARITY_EN_EVEN        1
#define USART_PARITY_EN_ODD         2

//USART_HWFlowControl
#define USART_HW_FLOW_CTRL_NONE     0
#define USART_HW_FLOW_CTRL_CTS      1
#define USART_HW_FLOW_CTRL_RTS      2
#define USART_HW_FLOW_CTRL_CTS_RTS  3

void USART_CLK_Control(USART_RegDef_t* pUSARTx, uint8_t EnorDi);
void USART_Init(USART_Handle_t* pUSARTHandle);
void USART_Reset(USART_RegDef_t* pUSARTx);

void USART_Tx(USART_RegDef_t* pUSARTx, uint8_t* pTxBuffer, uint32_t Len);
void USART_Rx(USART_RegDef_t* pUSARTx, uint8_t* pRxBuffer, uint32_t Len);

void USART_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void USART_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);

#endif