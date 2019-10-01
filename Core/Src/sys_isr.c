/*
 *  InterruptServiceRutine Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "sys_isr.h"

Isr_List_t SysIsr_List[ISR_NumberOfTypes];

/*********************************************************/
/*********************************************************/
/*********************************************************/
void SysIsr_Init(Core_Isr_Name_t source_isr)
{
    SysList_Init((list_t)&SysIsr_List[source_isr]);
}
/*********************************************************/
void SysIsr_Add(Isr_List_t *isr_handle, void *f_Ptr, Core_Isr_Name_t source_isr)
{
    if(SysList_Length((list_t)&SysIsr_List[source_isr]) == 0)
    {
        SysList_Init((list_t)&SysIsr_List[source_isr]);
    }
    isr_handle->vfPtr = (void	(*)(void))f_Ptr;
    SysList_Add((list_t)&SysIsr_List[source_isr], isr_handle);
}
/*********************************************************/
void SysIsr_Remove(Isr_List_t *isr_handle, Core_Isr_Name_t source_isr)
{
    SysList_Remove((list_t)&SysIsr_List[source_isr], isr_handle);
}
/*********************************************************/
void WWDG_IRQHandler(void)
{
    SYSISR_PROCCES(_WWDG_IRQHandler);
}
void PVD_PVM_IRQHandler(void)
{
    SYSISR_PROCCES(_PVD_PVM_IRQHandler);
}
void TAMP_STAMP_IRQHandler(void)
{
    SYSISR_PROCCES(_TAMP_STAMP_IRQHandler);
}
void FLASH_IRQHandler(void)
{
    SYSISR_PROCCES(_FLASH_IRQHandler);
}
void EXTI0_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI0_IRQHandler);
}
void EXTI1_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI1_IRQHandler);
}
void EXTI2_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI2_IRQHandler);
}
void EXTI3_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI3_IRQHandler);
}
void EXTI4_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI4_IRQHandler);
}
void DMA1_Channel1_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel1_IRQHandler);
}
void DMA1_Channel2_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel2_IRQHandler);
}
void DMA1_Channel3_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel3_IRQHandler);
}
void DMA1_Channel4_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel4_IRQHandler);
}
void DMA1_Channel5_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel5_IRQHandler);
}
void DMA1_Channel6_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel6_IRQHandler);
}
void DMA1_Channel7_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA1_Channel7_IRQHandler);
}
void ADC1_IRQHandler(void)
{
    SYSISR_PROCCES(_ADC1_IRQHandler);
}
void CAN1_TX_IRQHandler(void)
{
    SYSISR_PROCCES(_CAN1_TX_IRQHandler);
}
void CAN1_RX0_IRQHandler(void)
{
    SYSISR_PROCCES(_CAN1_RX0_IRQHandler);
}
void CAN1_RX1_IRQHandler(void)
{
    SYSISR_PROCCES(_CAN1_RX1_IRQHandler);
}
void CAN1_SCE_IRQHandler(void)
{
    SYSISR_PROCCES(_CAN1_SCE_IRQHandler);
}
void EXTI9_5_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI9_5_IRQHandler);
}
void TIM1_BRK_TIM15_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM1_BRK_TIM15_IRQHandler);
}
void TIM1_UP_TIM16_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM1_UP_TIM16_IRQHandler);
}
void TIM1_TRG_COM_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM1_TRG_COM_IRQHandler);
}
void TIM1_CC_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM1_CC_IRQHandler);
}
void TIM2_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM2_IRQHandler);
}
void TIM3_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM3_IRQHandler);
}
void I2C1_EV_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C1_EV_IRQHandler);
}
void I2C1_ER_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C1_ER_IRQHandler);
}
void I2C2_EV_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C2_EV_IRQHandler);
}
void I2C2_ER_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C2_ER_IRQHandler);
}
void SPI1_IRQHandler(void)
{
    SYSISR_PROCCES(_SPI1_IRQHandler);
}
void SPI2_IRQHandler(void)
{
    SYSISR_PROCCES(_SPI2_IRQHandler);
}
void USART1_IRQHandler(void)
{
    SYSISR_PROCCES(_USART1_IRQHandler);
}
void USART2_IRQHandler(void)
{
    SYSISR_PROCCES(_USART2_IRQHandler);
}
void USART3_IRQHandler(void)
{
    SYSISR_PROCCES(_USART3_IRQHandler);
}
void EXTI15_10_IRQHandler(void)
{
    SYSISR_PROCCES(_EXTI15_10_IRQHandler);
}
void RTC_Alarm_IRQHandler(void)
{
    SYSISR_PROCCES(_RTC_Alarm_IRQHandler);
}
void SDMMC1_IRQHandler(void)
{
    SYSISR_PROCCES(_SDMMC1_IRQHandler);
}
void SPI3_IRQHandler(void)
{
    SYSISR_PROCCES(_SPI3_IRQHandler);
}
void UART4_IRQHandler(void)
{
    SYSISR_PROCCES(_UART4_IRQHandler);
}
void TIM6_DAC_IRQHandler(void)
{
    SYSISR_PROCCES(_TIM6_DAC_IRQHandler);
}
void DMA2_Channel1_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel1_IRQHandler);
}
void DMA2_Channel2_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel2_IRQHandler);
}
void DMA2_Channel3_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel3_IRQHandler);
}
void DMA2_Channel4_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel4_IRQHandler);
}
void DMA2_Channel5_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel5_IRQHandler);
}
void DFSDM1_FLT0_IRQHandler(void)
{
    SYSISR_PROCCES(_DFSDM1_FLT0_IRQHandler);
}
void DFSDM1_FLT1_IRQHandler(void)
{
    SYSISR_PROCCES(_DFSDM1_FLT1_IRQHandler);
}
void COMP_IRQHandler(void)
{
    SYSISR_PROCCES(_COMP_IRQHandler);
}
void LPTIM1_IRQHandler(void)
{
    SYSISR_PROCCES(_LPTIM1_IRQHandler);
}
void LPTIM2_IRQHandler(void)
{
    SYSISR_PROCCES(_LPTIM2_IRQHandler);
}
void DMA2_Channel6_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel6_IRQHandler);
}
void DMA2_Channel7_IRQHandler(void)
{
    SYSISR_PROCCES(_DMA2_Channel7_IRQHandler);
}
void LPUART1_IRQHandler(void)
{
    SYSISR_PROCCES(_LPUART1_IRQHandler);
}
void QUADSPI_IRQHandler(void)
{
    SYSISR_PROCCES(_QUADSPI_IRQHandler);
}
void I2C3_EV_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C3_EV_IRQHandler);
}
void I2C3_ER_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C3_ER_IRQHandler);
}
void SAI1_IRQHandler(void)
{
    SYSISR_PROCCES(_SAI1_IRQHandler);
}
void TSC_IRQHandler(void)
{
    SYSISR_PROCCES(_TSC_IRQHandler);
}
void RNG_IRQHandler(void)
{
    SYSISR_PROCCES(_RNG_IRQHandler);
}
void FPU_IRQHandler(void)
{
    SYSISR_PROCCES(_FPU_IRQHandler);
}
void CRS_IRQHandler(void)
{
    SYSISR_PROCCES(_CRS_IRQHandler);
}
void I2C4_EV_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C4_EV_IRQHandler);
}
void I2C4_ER_IRQHandler(void)
{
    SYSISR_PROCCES(_I2C4_ER_IRQHandler);
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
