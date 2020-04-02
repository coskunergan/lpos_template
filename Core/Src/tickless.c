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

/*-----------------------------------------------------------*/

/* Calculate how many clock increments make up a single tick period. */
static const uint32_t ulReloadValueForOneTick = ((32768UL / configTICK_RATE_HZ) + 1);

/* Holds the maximum number of ticks that can be suppressed - which is
basically how far into the future an interrupt can be generated. Set during
initialisation. */
static TickType_t xMaximumPossibleSuppressedTicks = ((unsigned long) 0xFFFF) / ulReloadValueForOneTick;

/* Flag set from the tick interrupt to allow the sleep processing to know if
sleep mode was exited because of an tick interrupt or a different interrupt. */
static volatile uint32_t ulTickFlag = pdFALSE;

extern LPTIM_HandleTypeDef hlptim1;

/*-----------------------------------------------------------*/

/* The tick interrupt handler.  This is always the same other than the part that
clears the interrupt, which is specific to the clock being used to generate the
tick. */

void HAL_LPTIM_CompareMatchCallback(LPTIM_HandleTypeDef *hlptim)
{
    /* The next block of code is from the standard FreeRTOS tick interrupt
    handler.  The standard handler is not called directly in case future
    versions contain changes that make it no longer suitable for calling
    here. */
    (void) portSET_INTERRUPT_MASK_FROM_ISR();
    {
        if(xTaskIncrementTick() != pdFALSE)
        {
            portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT;
        }

        /* Just completely clear the interrupt mask on exit by passing 0 because
        it is known that this interrupt will only ever execute with the lowest
        possible interrupt priority. */
    }
    portCLEAR_INTERRUPT_MASK_FROM_ISR(0);

    /* The CPU woke because of a tick. */
    ulTickFlag = pdTRUE;
}

/*-----------------------------------------------------------*/

/* Override the default definition of vPortSuppressTicksAndSleep() that is
weakly defined in the FreeRTOS Cortex-M4 port layer with a version that manages
the LPTIM1 interrupt, as the tick is generated from LPTIM1 compare matches events. */

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    TickType_t ulReloadValue, xModifiableIdleTime, ulCompleteTickPeriods;
    uint32_t ulCounterValue, temp;

    if(xExpectedIdleTime > xMaximumPossibleSuppressedTicks)
    {
        xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
    }
    /*
     * Calculate the reload value required to wait xExpectedIdleTime tick periods.
     */
    ulReloadValue = xExpectedIdleTime * ulReloadValueForOneTick;
    /*
     * TODO: Any compensation for the time the timer is stopped
     */
    /*
     * Stop the timer momentarily
     */
    HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);

    /*
     * Enter a critical section but don’t use the taskENTER_CRITICAL()
     * method as that will mask interrupts that should exit sleep mode.
     */
    __disable_irq();
    __dsb(portSY_FULL_READ_WRITE);
    __isb(portSY_FULL_READ_WRITE);

    /*
     * tick flag is set to false before going to sleep
     */
    ulTickFlag = pdFALSE;

    /*
     * If a context switch is pending then abandon the low power entry as the
     * context switch might have been pended by an external interrupt that requires
     * processing.
     */
    eSleepModeStatus sleep_action = eTaskConfirmSleepModeStatus();
    if(sleep_action == eAbortSleep)
    {
        /* Restart the tick
        */
        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValueForOneTick, ulReloadValueForOneTick);
        /* Re - enable the interrupts
        */
        __enable_irq();
    }
    else if(sleep_action == eNoTasksWaitingTimeout)
    {
        /*
         * Let the application carry out pre-sleep processing
         */
        configPRE_SLEEP_PROCESSING(&xModifiableIdleTime);
        /*
         * Sleep
         */
        HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);
        /*
         * Let the application carry out post-sleep processing
         */
        configPOST_SLEEP_PROCESSING(&xModifiableIdleTime);
        /* Restart the tick
        */
        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValueForOneTick, ulReloadValueForOneTick);
        /* Re - enable the interrupts
        */
        __enable_irq();
    }
    else
    {
        /*
         * TODO: Need to update the tick count
         */
        /*
         * Restart the tick
         */
        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValueForOneTick, ulReloadValue);
        xModifiableIdleTime = xExpectedIdleTime;

        /*
         * Check for nullness of power_manager_instance before operating on it.
         * This is necessary to assure that we're not triggering the sleep mode processing
         * before the controller creates the power_manager instance.
         */

        /*
         * Let the application carry out pre-sleep processing
         */
        configPRE_SLEEP_PROCESSING(&xModifiableIdleTime);
        /*
         * Sleep
         */
        if(xModifiableIdleTime > (TickType_t)(0.03 * configTICK_RATE_HZ))
        {
            HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);	// Running if greater than 30ms.
        }
        else if(xModifiableIdleTime > 0)
        {
            HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI);	// Running if between 1 of 29ms.
        }
        /*
         * Let the application carry out post-sleep processing
         */
        configPOST_SLEEP_PROCESSING(&xModifiableIdleTime);

        /* retrieve counter value from LPTIM */
        {
            /* multiple readings are done to get a reliable counter register reading as described in the datasheet */
            do
            {
                temp = HAL_LPTIM_ReadCounter(&hlptim1);
                ulCounterValue = HAL_LPTIM_ReadCounter(&hlptim1);
            }
            while(temp != ulCounterValue);
        }

        /*
         * Stop low power timer.
         * The time the clock is stopped for is not accounted here, since
         * the clock is so slow. It is quite unlikely it is stopped for a complete count period.
         */
        HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);

        /*
         * Re-enable interrupts.
         */
        __enable_irq();
        __dsb(portSY_FULL_READ_WRITE);
        __isb(portSY_FULL_READ_WRITE);

        if(ulTickFlag != pdFALSE)
        {
            /*
             * Tick interrupt ended the sleep.
             */
            ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
            HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValueForOneTick, ulReloadValueForOneTick);
        }
        else
        {
            /*
             * Something other than the tick interrupt ended the sleep.
             */
            ulCompleteTickPeriods = ulCounterValue / ulReloadValueForOneTick;
            ulCounterValue %= ulReloadValueForOneTick;
            if(ulCounterValue == 0)
            {
                ulCounterValue = ulReloadValueForOneTick;
                ++ulCompleteTickPeriods;
            }
            HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValueForOneTick, ulCounterValue);
        }

        /*
         * Wind the tick forward by the number of tick periods that the MCU remained in a low power state.
         */
        vTaskStepTick(ulCompleteTickPeriods);
    }
}
#endif

