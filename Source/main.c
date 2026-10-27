#include <stdint.h>
#include "Stm32f407g_disc1_gpio.h"
#include "Stm32f407g_disc1_systick.h"
#include "Stm32f407g_disc1_tim.h"
#include "Stm32f407g_disc1_rcc.h"

int main(){
    // 1. Initialize SYSCLK to 16 MHz HSI
    RCC_Handle_t rcc_sys;
    rcc_sys.pRCC = RCC;
    rcc_sys.RCC_Config.RCC_ClockSource = RCC_SYSCLK_HSI;          //[cite: 18]
    rcc_sys.RCC_Config.RCC_AHBPrescaler = RCC_AHB_DIV2;           //[cite: 18]
    rcc_sys.RCC_Config.RCC_APB1Prescaler = RCC_APB_DIV1;          //[cite: 18]
    rcc_sys.RCC_Config.RCC_APB2Prescaler = RCC_APB_DIV1;          //[cite: 18]
    rcc_sys.RCC_Config.RCC_FlashLatency = RCC_FLASH_LATENCY_0WS;  //[cite: 18]
    
    RCC_Init(&rcc_sys);                                           //[cite: 17]

    // 2. Declare volatile variables to hold the clock frequencies
    __vo uint32_t hclk_freq = 0;
    __vo uint32_t pclk1_freq = 0;
    __vo uint32_t pclk2_freq = 0;

    // 3. Read the live clock values from your driver
    hclk_freq = RCC_GetHCLKValue();
    pclk1_freq = RCC_GetPCLK1Value();
    pclk2_freq = RCC_GetPCLK2Value();

    while(1){

    }
    return 0;
}
