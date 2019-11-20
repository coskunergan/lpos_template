/*
 *  Calendar Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_calendar.h"
#include "pin_defines.h"

#ifdef LIB_CALENDAR

#define CALENDAR_PREEMPTPRIORITY 	15
#define CALENDAR_SUBPRIORITY  	0

/* subsecond number of bits */
#define N_PREDIV_S                 10
/* Synchonuous prediv  */
#define PREDIV_S                  ((1<<N_PREDIV_S)-1)
/* Asynchonuous prediv   */
#define PREDIV_A                  (1<<(15-N_PREDIV_S))-1

RTC_HandleTypeDef RtcHandle;
static struct isr_list_t Calendar_Isr_Handle;
extern struct calendar_date date;

/*********************************************************/
void Calendar_ISR(void)
{
    HAL_RTCEx_WakeUpTimerIRQHandler(&RtcHandle);
}
/*********************************************************/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    calendar_add_second_to_date(&date);
}
/*********************************************************/
void Calendar_Hw_Init()
{
    RCC_OscInitTypeDef RCC_OscInitStruct;
    RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

    /*##-1- Configue the RTC clock soucre ######################################*/
    /* -a- Enable LSE Oscillator */
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /* -b- Select LSI as RTC clock source */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-2- Enable the RTC peripheral Clock ####################################*/
    /* Enable RTC Clock */
    __HAL_RCC_RTC_ENABLE();

    RtcHandle.Instance = RTC;

    RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
    RtcHandle.Init.AsynchPrediv = PREDIV_A; /* RTC_ASYNCH_PREDIV; */
    RtcHandle.Init.SynchPrediv = PREDIV_S; /* RTC_SYNCH_PREDIV; */
    RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
    RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
    RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;

    HAL_RTC_Init(&RtcHandle);

    /*Enable Direct Read of the calendar registers (not through Shadow) */
    HAL_RTCEx_EnableBypassShadow(&RtcHandle);

    /*##-3- Configure the RTC Wake up peripheral #################################*/
    /* Setting the Wake up time to 1 s
         If RTC_WAKEUPCLOCK_CK_SPRE_16BITS is selected, the frequency is 1Hz,
         this allows to get a wakeup time equal to 1 s if the ciunter is 0x0 */
    HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x0, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);	// 1sn
    //HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, 0x3FF, RTC_WAKEUPCLOCK_RTCCLK_DIV16);//~500mS

    /*##-4- Configure the NVIC for RTC WKUP ###################################*/
    SysIsr_Add(&Calendar_Isr_Handle, Calendar_ISR, _RTC_WKUP_IRQHandler);
    HAL_NVIC_SetPriority(RTC_WKUP_IRQn, CALENDAR_PREEMPTPRIORITY, CALENDAR_SUBPRIORITY);
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
/*********************************************************/
void Calendar_Hw_DeInit(void)
{
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
    HAL_RTC_DeInit(&RtcHandle);
}
/*********************************************************/
void Calendar_Enable(void)
{
    HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
}
/*********************************************************/
void Calendar_Disable(void)
{
    HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
