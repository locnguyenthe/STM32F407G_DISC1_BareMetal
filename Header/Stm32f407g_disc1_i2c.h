#ifndef _STM32F407G_DISC1_I2C_H_
#define _STM32F407G_DISC1_I2C_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

typedef struct{
    __vo uint32_t CR1;        // 0x00
    __vo uint32_t CR2;        // 0x04
    __vo uint32_t OAR1;       // 0x08
    __vo uint32_t OAR2;       // 0x0C
    __vo uint32_t DR;         // 0x10
    __vo uint32_t SR1;        // 0x14
    __vo uint32_t SR2;        // 0x18
    __vo uint32_t CCR;        // 0x1C
    __vo uint32_t TRISE;      // 0x20
    __vo uint32_t FLTR;       // 0x24
}I2C_RegDef_t;

#define I2C1_Base                   0x40005400u
#define I2C2_Base                   0x40005800u
#define I2C3_Base                   0x40005C00u

#define I2C1                        ((I2C_RegDef_t*)I2C1_Base)
#define I2C2                        ((I2C_RegDef_t*)I2C2_Base)
#define I2C3                        ((I2C_RegDef_t*)I2C3_Base)

typedef struct{
    uint32_t I2C_SCLSpeed;
    uint8_t  I2C_DeviceAddress;
    uint8_t  I2C_AckControl;
    uint8_t  I2C_FMDutyCycle;
}I2C_Config_t;

typedef struct{
    I2C_RegDef_t* pI2Cx;
    I2C_Config_t  I2C_Config;
}I2C_Handle_t;

// I2C_SCLSpeed
#define I2C_SCL_SPEED_SM            100000
#define I2C_SCL_SPEED_FM            400000

// I2C_AckControl
#define I2C_ACK_DISABLE             0
#define I2C_ACK_ENABLE              1

// I2C_FMDutyCycle
#define I2C_FM_DUTY_2               0
#define I2C_FM_DUTY_16_9            1

void I2C_CLK_Control(I2C_RegDef_t* pI2Cx, uint8_t EnorDi);
void I2C_Init(I2C_Handle_t* pI2CHandle);
void I2C_MasterTx(I2C_Handle_t* pI2CHandle, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddr);
void I2C_MasterRx(I2C_Handle_t* pI2CHandle, uint8_t* pRxBuffer, uint32_t Len, uint8_t SlaveAddr);

#endif