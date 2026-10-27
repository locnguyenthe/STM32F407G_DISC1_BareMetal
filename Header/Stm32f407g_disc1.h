#ifndef _STM32F407G_DISC1_H_
#define _STM32F407G_DISC1_H_

#include <stdint.h>

#define __vo                        volatile

#define FLASH_Base_Memory_Address   0x08000000u
#define SRAM1_Base_Memory_Address   0x20000000u
#define SRAM2_Base_Memory_Address   0x2001C000u
#define SRAM_Base_Memory_Address    SRAM1_Base_Memory_Address

#define AHB3_Base_Memory_Address    0xA0000000u
#define AHB2_Base_Memory_Address    0x50000000u
#define AHB1_Base_Memory_Address    0x40020000u
#define APB2_Base_Memory_Address    0x40010000u
#define APB1_Base_Memory_Address    0x40000000u

// ================= BASE ADDRESSES =================

// Cortex-M4 Core Peripherals Base Address
#define SYSTICK_Base                0xE000E010u

//FLASH Interface Base Address (on AHB1)
#define FLASH_Interface_Base        0x40023C00u

// APB1 Timer Base Addresses
#define TIM2_Base                   0x40000000u
#define TIM3_Base                   0x40000400u
#define TIM4_Base                   0x40000800u
#define TIM5_Base                   0x40000C00u
#define TIM6_Base                   0x40001000u
#define TIM7_Base                   0x40001400u

//GPIO Base Address
#define GPIOA_Base                  0x40020000u
#define GPIOB_Base                  0x40020400u
#define GPIOC_Base                  0x40020800u
#define GPIOD_Base                  0x40020C00u
#define GPIOE_Base                  0x40021000u
#define GPIOF_Base                  0x40021400u
#define GPIOG_Base                  0x40021800u
#define GPIOH_Base                  0x40021C00u
#define GPIOI_Base                  0x40022000u
#define GPIOJ_Base                  0x40022400u
#define GPIOK_Base                  0x40022800u

//RCC Base Address
#define RCC_Base                    0x40023800u

//NVIC definitions
#define NVIC_ISER0                  (__vo uint32_t*)0xE000E100u
#define NVIC_ISER1                  (__vo uint32_t*)0xE000E104u
#define NVIC_ISER2                  (__vo uint32_t*)0xE000E108u
#define NVIC_ISER3                  (__vo uint32_t*)0xE000E10Cu

#define NVIC_ICER0                  (__vo uint32_t*)0XE000E180u
#define NVIC_ICER1                  (__vo uint32_t*)0XE000E184u
#define NVIC_ICER2                  (__vo uint32_t*)0XE000E188u
#define NVIC_ICER3                  (__vo uint32_t*)0XE000E18Cu

#define NVIC_IPR0                   (__vo uint32_t*)0xE000E400u

// ================= REGISTER DEFINITIONS =================

//RCC 
typedef struct{
    __vo uint32_t CR;            // 0x00
    __vo uint32_t PLLCFGR;       // 0x04
    __vo uint32_t CFGR;          // 0x08
    __vo uint32_t CIR;           // 0x0C
    __vo uint32_t AHB1RSTR;      // 0x10
    __vo uint32_t AHB2RSTR;      // 0x14
    __vo uint32_t AHB3RSTR;      // 0x18
    uint32_t reserved0;          // 0x1C (Fixes the first gap!)
    __vo uint32_t APB1RSTR;      // 0x20
    __vo uint32_t APB2RSTR;      // 0x24
    uint32_t reserved1[2];       // 0x28, 0x2C
    __vo uint32_t AHB1ENR;       // 0x30 (Now perfectly aligned)
    __vo uint32_t AHB2ENR;       // 0x34
    __vo uint32_t AHB3ENR;       // 0x38
    uint32_t reserved2;          // 0x3C (Fixes the second gap!)
    __vo uint32_t APB1ENR;       // 0x40
    __vo uint32_t APB2ENR;       // 0x44
    uint32_t reserved3[2];       // 0x48, 0x4C
    __vo uint32_t AHB1LPENR;     // 0x50
    __vo uint32_t AHB2LPENR;     // 0x54
    __vo uint32_t AHB3LPENR;     // 0x58
    uint32_t reserved4;          // 0x5C (Fixes the third gap!)
    __vo uint32_t APB1LPENR;     // 0x60
    __vo uint32_t APB2LPENR;     // 0x64
    uint32_t reserved5[2];       // 0x68, 0x6C
    __vo uint32_t BDCR;          // 0x70
    __vo uint32_t CSR;           // 0x74
    uint32_t reserved6[2];       // 0x78, 0x7C
    __vo uint32_t SSCGR;         // 0x80
    __vo uint32_t PLLI2SCFGR;    // 0x84
    __vo uint32_t PLLSAICFGR;    // 0x88
    __vo uint32_t DCKCFGR;       // 0x8C
} RCC_RegDef_t;

// SysTick
typedef struct{
    __vo uint32_t CSR;              // 0x00 Control and Status Register
    __vo uint32_t RVR;              // 0x04 Reload Value Register
    __vo uint32_t CVR;              // 0x08 Current Value Register
    __vo uint32_t CALIB;            // 0x0C Calibration Value Register
}SYSTICK_RegDef_t;

//FLASH Register Definition
typedef struct{
    __vo uint32_t ACR;              // 0x00 Flash access control register
    __vo uint32_t KEYR;             // 0x04 Flash key register
    __vo uint32_t OPTKEYR;          // 0x08 Flash option key register
    __vo uint32_t SR;               // 0x0C Flash status register
    __vo uint32_t CR;               // 0x10 Flash control register
    __vo uint32_t OPTCR;            // 0x14 Flash option control register
}FLASH_RegDef_t;

// TIM (TIM2 to TIM7)
typedef struct{
    __vo uint32_t CR1;              // 0x00 Control register 1
    __vo uint32_t CR2;              // 0x04 Control register 2
    __vo uint32_t SMCR;             // 0x08 Slave mode control register
    __vo uint32_t DIER;             // 0x0C DMA/interrupt enable register
    __vo uint32_t SR;               // 0x10 Status register
    __vo uint32_t EGR;              // 0x14 Event generation register
    __vo uint32_t CCMR1;            // 0x18 Capture/compare mode register 1
    __vo uint32_t CCMR2;            // 0x1C Capture/compare mode register 2
    __vo uint32_t CCER;             // 0x20 Capture/compare enable register
    __vo uint32_t CNT;              // 0x24 Counter
    __vo uint32_t PSC;              // 0x28 Prescaler
    __vo uint32_t ARR;              // 0x2C Auto-reload register
    uint32_t      reserved0;        // 0x30 Reserved (RCR on TIM1/TIM8)
    __vo uint32_t CCR1;             // 0x34 Capture/compare register 1
    __vo uint32_t CCR2;             // 0x38 Capture/compare register 2
    __vo uint32_t CCR3;             // 0x3C Capture/compare register 3
    __vo uint32_t CCR4;             // 0x40 Capture/compare register 4
    uint32_t      reserved1;        // 0x44 Reserved (BDTR on TIM1/TIM8)
    __vo uint32_t DCR;              // 0x48 DMA control register
    __vo uint32_t DMAR;             // 0x4C DMA address for full transfer
    __vo uint32_t OR;               // 0x50 Option register (TIM2/TIM5)
}TIM_RegDef_t;

//GPIO
typedef struct{
    __vo uint32_t MODER;
    __vo uint32_t OTYPER;
    __vo uint32_t OSPEEDR;
    __vo uint32_t PUPDR;
    __vo uint32_t IDR;
    __vo uint32_t ODR;
    __vo uint32_t BSRR;
    __vo uint32_t LCKR;
    __vo uint32_t AFRL;
    __vo uint32_t AFRH;
}GPIO_RegDef_t;

//EXTI
typedef struct{
    __vo uint32_t IMR;
    __vo uint32_t EMR;
    __vo uint32_t RTSR;
    __vo uint32_t FTSR;
    __vo uint32_t SWIER;
    __vo uint32_t PR;
}EXTI_RegDef_t;

//SYSCFG
typedef struct{
    __vo uint32_t MEMRMP;
    __vo uint32_t PMC;
    __vo uint32_t EXTICR[4];
    __vo uint32_t CMPCR;
}SYSCFG_RegDef_t;

//IRQ number defines
typedef enum
{
  /* -------------------  Cortex-M4 Processor Exceptions Numbers  ------------------- */
  NonMaskableInt_IRQn         = -14,    /*!< 2 Non Maskable Interrupt                                          */
  HardFault_IRQn              = -13,    /*!< 3 HardFault Interrupt                                             */
  MemoryManagement_IRQn       = -12,    /*!< 4 Memory Management Interrupt                                     */
  BusFault_IRQn               = -11,    /*!< 5 Bus Fault Interrupt                                             */
  UsageFault_IRQn             = -10,    /*!< 6 Usage Fault Interrupt                                           */
  SVCall_IRQn                 = -5,     /*!< 11 SV Call Interrupt                                              */
  DebugMonitor_IRQn           = -4,     /*!< 12 Debug Monitor Interrupt                                        */
  PendSV_IRQn                 = -2,     /*!< 14 Pend SV Interrupt                                              */
  SysTick_IRQn                = -1,     /*!< 15 System Tick Interrupt                                          */

  /* -------------------  STM32 Specific Interrupt Numbers  ------------------------- */
  WWDG_IRQn                   = 0,      /*!< Window WatchDog Interrupt                                         */
  PVD_IRQn                    = 1,      /*!< PVD through EXTI Line detection Interrupt                         */
  TAMP_STAMP_IRQn             = 2,      /*!< Tamper and TimeStamp interrupts through the EXTI line             */
  RTC_WKUP_IRQn               = 3,      /*!< RTC Wakeup interrupt through the EXTI line                        */
  FLASH_IRQn                  = 4,      /*!< FLASH global Interrupt                                            */
  RCC_IRQn                    = 5,      /*!< RCC global Interrupt                                              */
  EXTI0_IRQn                  = 6,      /*!< EXTI Line0 Interrupt                                              */
  // ... skipping to position 18 ...
  ADC_IRQn                    = 18,     /*!< ADC1, ADC2 and ADC3 global Interrupts                             */
  // ... goes all the way down to IRQ 81 for F407 ...
} IRQn_Type;

// ================= PERIPHERALS DEFINITION =================

//RCC
#define RCC                         ((RCC_RegDef_t*)0x40023800u)

// SysTick
#define SYSTICK                     ((SYSTICK_RegDef_t*)SYSTICK_Base)

#define FLASH                       ((FLASH_RegDef_t*)FLASH_Interface_Base)

// TIM
#define TIM2                        ((TIM_RegDef_t*)TIM2_Base)
#define TIM3                        ((TIM_RegDef_t*)TIM3_Base)
#define TIM4                        ((TIM_RegDef_t*)TIM4_Base)
#define TIM5                        ((TIM_RegDef_t*)TIM5_Base)
#define TIM6                        ((TIM_RegDef_t*)TIM6_Base)
#define TIM7                        ((TIM_RegDef_t*)TIM7_Base)

//GPIO
#define GPIOA                       ((GPIO_RegDef_t*)GPIOA_Base)
#define GPIOB                       ((GPIO_RegDef_t*)GPIOB_Base)
#define GPIOC                       ((GPIO_RegDef_t*)GPIOC_Base)
#define GPIOD                       ((GPIO_RegDef_t*)GPIOD_Base)
#define GPIOE                       ((GPIO_RegDef_t*)GPIOE_Base)
#define GPIOF                       ((GPIO_RegDef_t*)GPIOF_Base)
#define GPIOG                       ((GPIO_RegDef_t*)GPIOG_Base)
#define GPIOH                       ((GPIO_RegDef_t*)GPIOH_Base)
#define GPIOI                       ((GPIO_RegDef_t*)GPIOI_Base)
#define GPIOJ                       ((GPIO_RegDef_t*)GPIOJ_Base)
#define GPIOK                       ((GPIO_RegDef_t*)GPIOK_Base)

//EXTI
#define EXTI                        ((EXTI_RegDef_t*)0x40013C00u)

//SYSCFG
#define SYSCFG                      ((SYSCFG_RegDef_t*)0x40013800u)

#define Enable                      1
#define Disable                     0
#define Set                         Enable
#define Reset                       Disable

// ================= IRQ CONFIG =================

#define IRQ_NO_EXTI0 		        6
#define IRQ_NO_EXTI1 		        7
#define IRQ_NO_EXTI2 		        8
#define IRQ_NO_EXTI3 		        9
#define IRQ_NO_EXTI4 		        10
#define IRQ_NO_EXTI9_5 		        23
#define IRQ_NO_TIM2                 28
#define IRQ_NO_TIM3                 29
#define IRQ_NO_TIM4                 30
#define IRQ_NO_TIM5                 50
#define IRQ_NO_TIM6_DAC             54
#define IRQ_NO_TIM7                 55
#define IRQ_NO_EXTI15_10 	        40
#define IRQ_NO_SPI1			        35
#define IRQ_NO_SPI2                 36
#define IRQ_NO_SPI3                 51
#define IRQ_NO_SPI4                 84
#define IRQ_NO_I2C1_EV              31
#define IRQ_NO_I2C1_ER              32
#define IRQ_NO_USART1	            37
#define IRQ_NO_USART2	            38
#define IRQ_NO_USART3	            39
#define IRQ_NO_UART4	            52
#define IRQ_NO_UART5	            53
#define IRQ_NO_USART6	            71


#endif