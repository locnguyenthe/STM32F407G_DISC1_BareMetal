#ifndef _STM32F407G_DISC1_GPIO_H_
#define _STM32F407G_DISC1_GPIO_H_

#include <stdint.h>
#include "Stm32f407g_disc1.h"

typedef struct{
    uint8_t GPIO_PinNumber;
    uint8_t GPIO_PinMode;
    uint8_t GPIO_PinSpeed;
    uint8_t GPIO_PinPUPD;
    uint8_t GPIO_OutputType;
    uint8_t GPIO_PinAltFunc;
}GPIO_PinConfig_t;

typedef struct{
    GPIO_RegDef_t* pGPIOx;
    GPIO_PinConfig_t GPIO_PinConfig;
}GPIO_Handle_t;

//GPIO_PinNumber
#define GPIO_PIN_0                      0
#define GPIO_PIN_1                      1
#define GPIO_PIN_2                      2
#define GPIO_PIN_3                      3
#define GPIO_PIN_4                      4
#define GPIO_PIN_5                      5
#define GPIO_PIN_6                      6
#define GPIO_PIN_7                      7
#define GPIO_PIN_8                      8
#define GPIO_PIN_9                      9
#define GPIO_PIN_10                     10
#define GPIO_PIN_11                     11
#define GPIO_PIN_12                     12
#define GPIO_PIN_13                     13
#define GPIO_PIN_14                     14
#define GPIO_PIN_15                     15

//GPIO_PinMode
#define GPIO_MODE_IN                    0
#define GPIO_MODE_OUT                   1
#define GPIO_MODE_ALTFUNC               2
#define GPIO_MODE_ANALOG                3
#define GPIO_MODE_IT_FT                 4
#define GPIO_MODE_IT_RT                 5
#define GPIO_MODE_IT_FT_RT              6

//GPIO_OutputType
#define GPIO_PushPull                   0
#define GPIO_OpenDrain                  1

//GPIO_PinSpeed
#define GPIO_Speed_LOW                  0
#define GPIO_Speed_MEDIUM               1
#define GPIO_Speed_FAST                 2
#define GPIO_Speed_HIGH                 3

//GPIO_PinPUPD
#define GPIO_NO_Pullup_Pulldown         0
#define GPIO_Pullup                     1
#define GPIO_Pulldown                   2

void GPIO_CLK_Control(GPIO_RegDef_t* pGPIOx, uint8_t EnorDi);

void GPIO_Init(GPIO_Handle_t* pGPIOHandle);
void GPIO_Reset(GPIO_RegDef_t* pGPIOx);

uint8_t GPIO_Read(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);
void GPIO_Write(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber, uint8_t value);
void GPIO_WriteToggle(GPIO_RegDef_t* pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterrupt_Config(uint8_t IRQNumber, uint8_t EnorDi);
void GPIO_IRQInterrupt_Priority(uint8_t IRQNumber, uint8_t IRQPriority);
void GPIO_IRQInterrupt_Handling(uint8_t PinNumber);


#endif