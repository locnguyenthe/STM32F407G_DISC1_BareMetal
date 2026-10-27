#include "Stm32f407g_disc1_i2c.h"
#include "Stm32f407g_disc1_rcc.h"

void I2C_CLK_Control(I2C_RegDef_t* pI2Cx, uint8_t EnorDi){
    if(EnorDi == Enable){
        if(pI2Cx == I2C1) RCC->APB1ENR |= (1 << 21);
        else if(pI2Cx == I2C2) RCC->APB1ENR |= (1 << 22);
        else if(pI2Cx == I2C3) RCC->APB1ENR |= (1 << 23);
    } else {
        if(pI2Cx == I2C1) RCC->APB1ENR &= ~(1 << 21);
        else if(pI2Cx == I2C2) RCC->APB1ENR &= ~(1 << 22);
        else if(pI2Cx == I2C3) RCC->APB1ENR &= ~(1 << 23);
    }
}

void I2C_Init(I2C_Handle_t* pI2CHandle){
    I2C_CLK_Control(pI2CHandle->pI2Cx, Enable);
    uint32_t temp = 0;

    // 1. Configure ACK
    temp |= (pI2CHandle->I2C_Config.I2C_AckControl << 10);
    pI2CHandle->pI2Cx->CR1 = temp;

    // 2. Configure FREQ in CR2
    uint32_t pclk1 = RCC_GetPCLK1Value() / 1000000;
    pI2CHandle->pI2Cx->CR2 |= (pclk1 & 0x3F);

    // 3. Configure Device Address
    temp = (pI2CHandle->I2C_Config.I2C_DeviceAddress << 1);
    temp |= (1 << 14); // Required by manual
    pI2CHandle->pI2Cx->OAR1 = temp;

    // 4. Configure CCR setup for SCL speed
    uint16_t ccr_value = 0;
    if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
        ccr_value = (RCC_GetPCLK1Value() / (2 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        pI2CHandle->pI2Cx->CCR = (ccr_value & 0xFFF);
        pI2CHandle->pI2Cx->TRISE = (pclk1 + 1) & 0x3F;
    } else {
        temp = (1 << 15); // Fast mode
        temp |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
        if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2)
            ccr_value = (RCC_GetPCLK1Value() / (3 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        else
            ccr_value = (RCC_GetPCLK1Value() / (25 * pI2CHandle->I2C_Config.I2C_SCLSpeed));
        
        temp |= (ccr_value & 0xFFF);
        pI2CHandle->pI2Cx->CCR = temp;
        pI2CHandle->pI2Cx->TRISE = ((pclk1 * 300) / 1000) + 1;
    }
    
    // Enable I2C peripheral
    pI2CHandle->pI2Cx->CR1 |= (1 << 0);
}

// Master Tx and Rx implementations involve sequence handling: 
// START, ADDR, TXE/RXNE, STOP. (Stubbed for brevity, matching USART level)
void I2C_MasterTx(I2C_Handle_t* pI2CHandle, uint8_t* pTxBuffer, uint32_t Len, uint8_t SlaveAddr){
    pI2CHandle->pI2Cx->CR1 |= (1 << 8); // Generate START
    while(!(pI2CHandle->pI2Cx->SR1 & (1 << 0))); // Wait SB
    
    pI2CHandle->pI2Cx->DR = (SlaveAddr << 1); // Send Address with W
    while(!(pI2CHandle->pI2Cx->SR1 & (1 << 1))); // Wait ADDR
    uint32_t dummy = pI2CHandle->pI2Cx->SR1 | pI2CHandle->pI2Cx->SR2; // Clear ADDR
    (void)dummy;
    
    while(Len > 0){
        while(!(pI2CHandle->pI2Cx->SR1 & (1 << 7))); // Wait TXE
        pI2CHandle->pI2Cx->DR = *pTxBuffer;
        pTxBuffer++;
        Len--;
    }
    
    while(!(pI2CHandle->pI2Cx->SR1 & (1 << 7))); // TXE
    while(!(pI2CHandle->pI2Cx->SR1 & (1 << 2))); // BTF
    pI2CHandle->pI2Cx->CR1 |= (1 << 9); // STOP
}

void I2C_MasterRx(I2C_Handle_t* pI2CHandle, uint8_t* pRxBuffer, uint32_t Len, uint8_t SlaveAddr){
    // Read sequence implementation (Left as boilerplate placeholder)
}