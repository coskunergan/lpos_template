/*
 * FreeRTOS Kernel V10.2.1
 * Copyright (C) 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


/* Standard includes. */
#include <limits.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* ST library functions. */
#include "stm32l4xx_hal.h"

#if( configUSE_LPTIMER_TICKLESS_IDLE == 1 )

#define PIN_TRIG(GPIOx,PINx)  GPIOx->BRR=GPIOx->BSRR=PINx//TEST

/* The frequency at which LpTIMx will run. */
#define lpCLOCK_INPUT_FREQUENCY 	( 32768UL )

/* Running stop mod if greater than 3ms of the expected idle time */
#define lpSTOP_MODE_LIMIT_S ( 0.003 )

#define portNVIC_SYSTICK_LOAD_REG			( * ( ( volatile uint32_t * ) 0xe000e014 ) )
#define portNVIC_SYSTICK_CURRENT_VALUE_REG	( * ( ( volatile uint32_t * ) 0xe000e018 ) )

/* Calculate how many clock increments make up a single tick period. */
static const uint32_t ulReloadValueForOneTick = (lpCLOCK_INPUT_FREQUENCY / configTICK_RATE_HZ);

/* Holds the maximum number of ticks that can be suppressed - which is
basically how far into the future an interrupt can be generated. Set during
initialisation. */
static TickType_t xMaximumPossibleSuppressedTicks = ((unsigned long) USHRT_MAX) / ulReloadValueForOneTick;

/* Flag set from the tick interrupt to allow the sleep processing to know if
sleep mode was exited because of an tick interrupt or a different interrupt. */
static volatile uint32_t ulTickFlag = pdFALSE;

static uint32_t xReloadIdleTime = 0;

extern LPTIM_HandleTypeDef hlptim1;
/*************************************************************/
/*************************************************************/
/*************************************************************/
void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    PIN_TRIG(GPIOA, GPIO_PIN_0);
    ulTickFlag = pdTRUE;
}
/*************************************************************/
void PreSleepProcessing(uint32_t *ulExpectedIdleTime)
{
    if(*ulExpectedIdleTime > (lpSTOP_MODE_LIMIT_S * configTICK_RATE_HZ))
    {
        PIN_TRIG(GPIOA, GPIO_PIN_0);

        xReloadIdleTime = *ulExpectedIdleTime;

        *ulExpectedIdleTime = 0;

        if(xReloadIdleTime > xMaximumPossibleSuppressedTicks)
        {
            xReloadIdleTime = xMaximumPossibleSuppressedTicks;
        }

        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, USHRT_MAX, xReloadIdleTime * ulReloadValueForOneTick);

        ulTickFlag = pdFALSE;

        PIN_TRIG(GPIOA, GPIO_PIN_1);

        //HAL_PWREx_EnterSTOP0Mode(PWR_SLEEPENTRY_WFI);// 140uA idle current
        //HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI); // 32uA idle current
        HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI); // 6.7uA idle current
    }
    else
    {
        xReloadIdleTime = 0;
    }
}
/*************************************************************/
void PostSleepProcessing(uint32_t *ulExpectedIdleTime)
{
    uint32_t ulCountAfterSleep, temp;

    if(xReloadIdleTime > 0)
    {
        __enable_irq();
        __dsb(portSY_FULL_READ_WRITE);
        __isb(portSY_FULL_READ_WRITE);

        do
        {
            ulCountAfterSleep = HAL_LPTIM_ReadCounter(&hlptim1);
        }
        while(ulCountAfterSleep != HAL_LPTIM_ReadCounter(&hlptim1));
        HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);

        __disable_irq();
        __dsb(portSY_FULL_READ_WRITE);
        __isb(portSY_FULL_READ_WRITE);

        if(ulTickFlag != pdFALSE)
        {
						// wakeup from lp timer interrput.
            PIN_TRIG(GPIOA, GPIO_PIN_3); // timer kesme ile uyandi
            portNVIC_SYSTICK_LOAD_REG = 0UL;
        }
        else
        {
						// wakeup from externel interrput.
            PIN_TRIG(GPIOA, GPIO_PIN_4);  
            temp = ((ulCountAfterSleep * (configCPU_CLOCK_HZ / configTICK_RATE_HZ)) / ulReloadValueForOneTick);
            if(portNVIC_SYSTICK_CURRENT_VALUE_REG > temp)
            {
                portNVIC_SYSTICK_LOAD_REG	= portNVIC_SYSTICK_CURRENT_VALUE_REG - temp;
            }
            else
            {
                portNVIC_SYSTICK_LOAD_REG = 0UL;
            }
        }
        portNVIC_SYSTICK_CURRENT_VALUE_REG = 0UL;
    }
}
/*************************************************************/
/*************************************************************/
/*************************************************************/
#endif
