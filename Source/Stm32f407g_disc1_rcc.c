#include <stdint.h>
#include "Stm32f407g_disc1_rcc.h"

static const uint16_t AHB_PreScaler[8] = {2, 4, 8, 16, 64, 128, 256, 512};
static const uint8_t  APB_PreScaler[4] = {2, 4, 8, 16};

void RCC_Init(RCC_Handle_t* pRCCHandle){
    // 1. Configure Flash Latency (Wait States) and enable Prefetch, I-Cache, D-Cache
    FLASH->ACR &= ~(0x07 << 0);
    FLASH->ACR |= (pRCCHandle->RCC_Config.RCC_FlashLatency & 0x07);
    FLASH->ACR |= ((1 << 8) | (1 << 9) | (1 << 10));

    // 2. Configure Bus Prescalers (AHB, APB1, APB2) before switching to high-speed clock
    pRCCHandle->pRCC->CFGR &= ~(0x0F << 4);
    pRCCHandle->pRCC->CFGR |= ((pRCCHandle->RCC_Config.RCC_AHBPrescaler & 0x0F) << 4);

    pRCCHandle->pRCC->CFGR &= ~(0x07 << 10);
    pRCCHandle->pRCC->CFGR |= ((pRCCHandle->RCC_Config.RCC_APB1Prescaler & 0x07) << 10);

    pRCCHandle->pRCC->CFGR &= ~(0x07 << 13);
    pRCCHandle->pRCC->CFGR |= ((pRCCHandle->RCC_Config.RCC_APB2Prescaler & 0x07) << 13);

    // 3. Enable and switch to the selected Clock Source
    if (pRCCHandle->RCC_Config.RCC_ClockSource == RCC_SYSCLK_HSI){
        pRCCHandle->pRCC->CR |= (1 << 0);                   // Enable HSI (HSION)
        while (!(pRCCHandle->pRCC->CR & (1 << 1)));         // Wait for HSIRDY

        pRCCHandle->pRCC->CFGR &= ~(0x03 << 0);             // Select HSI as SYSCLK (SW = 00)
        while (((pRCCHandle->pRCC->CFGR >> 2) & 0x03) != RCC_SYSCLK_HSI);
    }
    else if (pRCCHandle->RCC_Config.RCC_ClockSource == RCC_SYSCLK_HSE){
        pRCCHandle->pRCC->CR |= (1 << 16);                  // Enable HSE (HSEON)
        while (!(pRCCHandle->pRCC->CR & (1 << 17)));        // Wait for HSERDY

        pRCCHandle->pRCC->CFGR &= ~(0x03 << 0);
        pRCCHandle->pRCC->CFGR |= (RCC_SYSCLK_HSE << 0);    // Select HSE as SYSCLK (SW = 01)
        while (((pRCCHandle->pRCC->CFGR >> 2) & 0x03) != RCC_SYSCLK_HSE);
    }
    else if (pRCCHandle->RCC_Config.RCC_ClockSource == RCC_SYSCLK_PLL){
        // Disable PLL before configuring PLLCFGR
        pRCCHandle->pRCC->CR &= ~(1 << 24);
        while (pRCCHandle->pRCC->CR & (1 << 25));

        // Enable PLL input oscillator
        if (pRCCHandle->RCC_Config.RCC_PLLSource == RCC_PLL_SOURCE_HSE){
            pRCCHandle->pRCC->CR |= (1 << 16);              // Enable HSE
            while (!(pRCCHandle->pRCC->CR & (1 << 17)));    // Wait for HSERDY
        }
        else{
            pRCCHandle->pRCC->CR |= (1 << 0);               // Enable HSI
            while (!(pRCCHandle->pRCC->CR & (1 << 1)));     // Wait for HSIRDY
        }

        // Configure PLLCFGR (PLLM[5:0], PLLN[14:6], PLLP[17:16], PLLSRC[22])
        pRCCHandle->pRCC->PLLCFGR &= ~((0x3F << 0) | (0x1FF << 6) | (0x03 << 16) | (1 << 22));
        pRCCHandle->pRCC->PLLCFGR |= ((pRCCHandle->RCC_Config.RCC_PLLM & 0x3F) << 0);
        pRCCHandle->pRCC->PLLCFGR |= ((pRCCHandle->RCC_Config.RCC_PLLN & 0x1FF) << 6);
        pRCCHandle->pRCC->PLLCFGR |= ((pRCCHandle->RCC_Config.RCC_PLLP & 0x03) << 16);
        pRCCHandle->pRCC->PLLCFGR |= ((pRCCHandle->RCC_Config.RCC_PLLSource & 0x01) << 22);

        // Enable PLL and wait until locked
        pRCCHandle->pRCC->CR |= (1 << 24);                  // PLLON = 1
        while (!(pRCCHandle->pRCC->CR & (1 << 25)));        // Wait for PLLRDY

        // Switch SYSCLK to PLL
        pRCCHandle->pRCC->CFGR &= ~(0x03 << 0);
        pRCCHandle->pRCC->CFGR |= (RCC_SYSCLK_PLL << 0);    // Select PLL as SYSCLK (SW = 10)
        while (((pRCCHandle->pRCC->CFGR >> 2) & 0x03) != RCC_SYSCLK_PLL);
    }
}

uint32_t RCC_GetSYSCLKValue(void){
    uint8_t clkSrc = (RCC->CFGR >> 2) & 0x03;

    if (clkSrc == RCC_SYSCLK_HSI){
        return HSI_VALUE;
    }
    else if (clkSrc == RCC_SYSCLK_HSE){
        return HSE_VALUE;
    }
    else if (clkSrc == RCC_SYSCLK_PLL){
        uint32_t pllInput = (RCC->PLLCFGR & (1 << 22)) ? HSE_VALUE : HSI_VALUE;
        uint32_t pllm = RCC->PLLCFGR & 0x3F;
        uint32_t plln = (RCC->PLLCFGR >> 6) & 0x1FF;
        uint32_t pllp = (((RCC->PLLCFGR >> 16) & 0x03) + 1) * 2;

        return ((pllInput / pllm) * plln) / pllp;
    }
    return HSI_VALUE;
}

uint32_t RCC_GetHCLKValue(void){
    uint32_t sysclk = RCC_GetSYSCLKValue();
    uint8_t temp = (RCC->CFGR >> 4) & 0x0F;
    uint16_t ahbPrescaler = (temp < 8) ? 1 : AHB_PreScaler[temp - 8];

    return sysclk / ahbPrescaler;
}

uint32_t RCC_GetPCLK1Value(void){
    uint32_t hclk = RCC_GetHCLKValue();
    uint8_t temp = (RCC->CFGR >> 10) & 0x07;
    uint8_t apb1Prescaler = (temp < 4) ? 1 : APB_PreScaler[temp - 4];

    return hclk / apb1Prescaler;
}

uint32_t RCC_GetPCLK2Value(void){
    uint32_t hclk = RCC_GetHCLKValue();
    uint8_t temp = (RCC->CFGR >> 13) & 0x07;
    uint8_t apb2Prescaler = (temp < 4) ? 1 : APB_PreScaler[temp - 4];

    return hclk / apb2Prescaler;
}