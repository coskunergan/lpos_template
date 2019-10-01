/*
 *  InterruptServiceRutine Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */


#ifndef SYS_ISR_H_
#define SYS_ISR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

struct isr_list_t
{
    struct  isr_list_t *next;
    void	(*vfPtr)(void);
};
typedef struct isr_list_t Isr_List_t;


#define SYSISR_PROCCES(_X_IRQ_Handler) 													\
		extern Isr_List_t SysIsr_List[];														\
    static struct isr_list_t *s; 																\
    for(s = SysList_Head((list_t)&SysIsr_List[_X_IRQ_Handler]); \
            s != NULL; 																					\
            s = SysList_Item_Next(s)) 													\
    {																														\
        s->vfPtr(); 																						\
    }


typedef enum
{
    _NMI_Handler
    , _HardFault_Handler
    , _MemManage_Handler
    , _BusFault_Handler
    , _UsageFault_Handler
    , _SVC_Handler
    , _DebugMon_Handler
    , _PendSV_Handler
    , _SysTick_Handler
    , _WWDG_IRQHandler
    , _PVD_PVM_IRQHandler
    , _TAMP_STAMP_IRQHandler
    , _RTC_WKUP_IRQHandler //
    , _FLASH_IRQHandler
    , _RCC_IRQHandler //
    , _EXTI0_IRQHandler
    , _EXTI1_IRQHandler
    , _EXTI2_IRQHandler
    , _EXTI3_IRQHandler
    , _EXTI4_IRQHandler
    , _DMA1_Channel1_IRQHandler
    , _DMA1_Channel2_IRQHandler
    , _DMA1_Channel3_IRQHandler
    , _DMA1_Channel4_IRQHandler
    , _DMA1_Channel5_IRQHandler
    , _DMA1_Channel6_IRQHandler
    , _DMA1_Channel7_IRQHandler
    , _ADC1_IRQHandler
    , _CAN1_TX_IRQHandler
    , _CAN1_RX0_IRQHandler
    , _CAN1_RX1_IRQHandler
    , _CAN1_SCE_IRQHandler
    , _EXTI9_5_IRQHandler
    , _TIM1_BRK_TIM15_IRQHandler
    , _TIM1_UP_TIM16_IRQHandler
    , _TIM1_TRG_COM_IRQHandler
    , _TIM1_CC_IRQHandler
    , _TIM2_IRQHandler
    , _TIM3_IRQHandler
    , _I2C1_EV_IRQHandler
    , _I2C1_ER_IRQHandler
    , _I2C2_EV_IRQHandler
    , _I2C2_ER_IRQHandler
    , _SPI1_IRQHandler
    , _SPI2_IRQHandler
    , _USART1_IRQHandler
    , _USART2_IRQHandler
    , _USART3_IRQHandler
    , _EXTI15_10_IRQHandler
    , _RTC_Alarm_IRQHandler
    , _SDMMC1_IRQHandler
    , _SPI3_IRQHandler
    , _UART4_IRQHandler
    , _TIM6_DAC_IRQHandler
    , _DMA2_Channel1_IRQHandler
    , _DMA2_Channel2_IRQHandler
    , _DMA2_Channel3_IRQHandler
    , _DMA2_Channel4_IRQHandler
    , _DMA2_Channel5_IRQHandler
    , _DFSDM1_FLT0_IRQHandler
    , _DFSDM1_FLT1_IRQHandler
    , _COMP_IRQHandler
    , _LPTIM1_IRQHandler
    , _LPTIM2_IRQHandler
    , _DMA2_Channel6_IRQHandler
    , _DMA2_Channel7_IRQHandler
    , _LPUART1_IRQHandler
    , _QUADSPI_IRQHandler
    , _I2C3_EV_IRQHandler
    , _I2C3_ER_IRQHandler
    , _SAI1_IRQHandler
    , _TSC_IRQHandler
    , _RNG_IRQHandler
    , _FPU_IRQHandler
    , _CRS_IRQHandler
    , _I2C4_EV_IRQHandler
    , _I2C4_ER_IRQHandler
    , ISR_NumberOfTypes
} Core_Isr_Name_t;

void SysIsr_Start(void);
void SysIsr_Init(Core_Isr_Name_t source_isr);
void SysIsr_Add(Isr_List_t *isr_handle, void *f_Ptr, Core_Isr_Name_t source_isr);
void SysIsr_Remove(Isr_List_t *isr_handle, Core_Isr_Name_t source_isr);

#ifdef __cplusplus
}
#endif

#endif /* SYS_ISR_H_ */

