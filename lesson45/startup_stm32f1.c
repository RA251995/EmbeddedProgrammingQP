#include <stdint.h>
#include "stm32f4_cmsis.h"

typedef void (*FPTR_T)(void);

extern uint32_t __INITIAL_SP;
extern uint32_t Reset_Handler;

void Unused_Handler(void);

FPTR_T const __VECTOR_TABLE_ATTRIBUTE __VECTOR_TABLE[256] = {
    (FPTR_T)&__INITIAL_SP,
    (FPTR_T)&Reset_Handler,
    &NMI_Handler,
    &HardFault_Handler,
    &MemManage_Handler,
    &BusFault_Handler,
    &UsageFault_Handler,
    0, /* Reserved */
    0, /* Reserved */
    0, /* Reserved */
    0, /* Reserved */
    &SVC_Handler,
    &DebugMon_Handler,
    0, /* Reserved */
    &PendSV_Handler,
    &SysTick_Handler,
    &WWDG_IRQHandler,
    &PVD_IRQHandler,
    &TAMPER_IRQHandler,
    &RTC_IRQHandler,
    &FLASH_IRQHandler,
    &RCC_IRQHandler,
    &EXTI0_IRQHandler,
    &EXTI1_IRQHandler,
    &EXTI2_IRQHandler,
    &EXTI3_IRQHandler,
    &EXTI4_IRQHandler,
    &DMA1_Channel1_IRQHandler,
    &DMA1_Channel2_IRQHandler,
    &DMA1_Channel3_IRQHandler,
    &DMA1_Channel4_IRQHandler,
    &DMA1_Channel5_IRQHandler,
    &DMA1_Channel6_IRQHandler,
    &DMA1_Channel7_IRQHandler,
    &ADC1_2_IRQHandler,
    &USB_HP_CAN_TX_IRQHandler,
    &USB_LP_CAN_RX0_IRQHandler,
    &CAN_RX1_IRQHandler,
    &CAN_SCE_IRQHandler,
    &EXTI9_5_IRQHandler,
    &TIM1_BRK_IRQHandler,
    &TIM1_UP_IRQHandler,
    &TIM1_TRG_COM_IRQHandler,
    &TIM1_CC_IRQHandler,
    &TIM2_IRQHandler,
    &TIM3_IRQHandler,
    &TIM4_IRQHandler,
    &I2C1_EV_IRQHandler,
    &I2C1_ER_IRQHandler,
    &I2C2_EV_IRQHandler,
    &I2C2_ER_IRQHandler,
    &SPI1_IRQHandler,
    &SPI2_IRQHandler,
    &USART1_IRQHandler,
    &USART2_IRQHandler,
    &USART3_IRQHandler,
    &EXTI15_10_IRQHandler,
    &RTCAlarm_IRQHandler,
    &USBWakeup_IRQHandler,
    &TIM8_BRK_IRQHandler,
    &TIM8_UP_IRQHandler,
    &TIM8_TRG_COM_IRQHandler,
    &TIM8_CC_IRQHandler,
    &ADC3_IRQHandler,
    &FSMC_IRQHandler,
    &SDIO_IRQHandler,
    &TIM5_IRQHandler,
    &SPI3_IRQHandler,
    &UART4_IRQHandler,
    &UART5_IRQHandler,
    &TIM6_IRQHandler,
    &TIM7_IRQHandler,
    &DMA2_Channel1_IRQHandler,
    &DMA2_Channel2_IRQHandler,
    &DMA2_Channel3_IRQHandler,
    &DMA2_Channel4_5_IRQHandler
};

void __NO_RETURN NMI_Handler(void)
{
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
}

void __NO_RETURN HardFault_Handler(void)
{
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
}

void __NO_RETURN MemManage_Handler(void)
{
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
}

void __NO_RETURN BusFault_Handler(void)
{
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
}

void __NO_RETURN UsageFault_Handler(void)
{
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
}

void __NO_RETURN Unused_Handler(void)
{
    /* For debugging, hang on in an endless loop... */
    for (;;)
    {
    }
}

void SVC_Handler(void)                __WEAK __ALIAS("Unused_Handler");
void DebugMon_Handler(void)           __WEAK __ALIAS("Unused_Handler");
void PendSV_Handler(void)             __WEAK __ALIAS("Unused_Handler");
void SysTick_Handler(void)            __WEAK __ALIAS("Unused_Handler");
void WWDG_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void PVD_IRQHandler(void)             __WEAK __ALIAS("Unused_Handler");
void TAMPER_IRQHandler(void)          __WEAK __ALIAS("Unused_Handler");
void RTC_IRQHandler(void)             __WEAK __ALIAS("Unused_Handler");
void FLASH_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void RCC_IRQHandler(void)             __WEAK __ALIAS("Unused_Handler");
void EXTI0_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void EXTI1_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void EXTI2_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void EXTI3_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void EXTI4_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel1_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel2_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel3_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel4_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel5_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel6_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA1_Channel7_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void ADC1_2_IRQHandler(void)          __WEAK __ALIAS("Unused_Handler");
void USB_HP_CAN_TX_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void USB_LP_CAN_RX0_IRQHandler(void)  __WEAK __ALIAS("Unused_Handler");
void CAN_RX1_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void CAN_SCE_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void EXTI9_5_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void TIM1_BRK_IRQHandler(void)        __WEAK __ALIAS("Unused_Handler");
void TIM1_UP_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void TIM1_TRG_COM_IRQHandler(void)    __WEAK __ALIAS("Unused_Handler");
void TIM1_CC_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void TIM2_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void TIM3_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void TIM4_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void I2C1_EV_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void I2C1_ER_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void I2C2_EV_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void I2C2_ER_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void SPI1_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void SPI2_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void USART1_IRQHandler(void)          __WEAK __ALIAS("Unused_Handler");
void USART2_IRQHandler(void)          __WEAK __ALIAS("Unused_Handler");
void USART3_IRQHandler(void)          __WEAK __ALIAS("Unused_Handler");
void EXTI15_10_IRQHandler(void)       __WEAK __ALIAS("Unused_Handler");
void RTCAlarm_IRQHandler(void)        __WEAK __ALIAS("Unused_Handler");
void USBWakeup_IRQHandler(void)       __WEAK __ALIAS("Unused_Handler");
void TIM8_BRK_IRQHandler(void)        __WEAK __ALIAS("Unused_Handler");
void TIM8_UP_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void TIM8_TRG_COM_IRQHandler(void)    __WEAK __ALIAS("Unused_Handler");
void TIM8_CC_IRQHandler(void)         __WEAK __ALIAS("Unused_Handler");
void ADC3_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void FSMC_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void SDIO_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void TIM5_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void SPI3_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void UART4_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void UART5_IRQHandler(void)           __WEAK __ALIAS("Unused_Handler");
void TIM6_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void TIM7_IRQHandler(void)            __WEAK __ALIAS("Unused_Handler");
void DMA2_Channel1_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA2_Channel2_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA2_Channel3_IRQHandler(void)   __WEAK __ALIAS("Unused_Handler");
void DMA2_Channel4_5_IRQHandler(void) __WEAK __ALIAS("Unused_Handler");
