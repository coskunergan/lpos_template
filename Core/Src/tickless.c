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
/* The frequency at which TIMx will run. */
#define lpCLOCK_INPUT_FREQUENCY 	( 32768UL )

/* Calculate how many clock increments make up a single tick period. */
static const uint32_t ulReloadValueForOneTick = ((lpCLOCK_INPUT_FREQUENCY / configTICK_RATE_HZ) - 1);

/* Holds the maximum number of ticks that can be suppressed - which is
basically how far into the future an interrupt can be generated. Set during
initialisation. */
static TickType_t xMaximumPossibleSuppressedTicks = ((unsigned long) USHRT_MAX) / ulReloadValueForOneTick;

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
    /* The CPU woke because of a tick. */
    ulTickFlag = pdTRUE;

    __HAL_LPTIM_COMPARE_SET(&hlptim1, (uint16_t)ulReloadValueForOneTick);
    __HAL_LPTIM_AUTORELOAD_SET(&hlptim1, (uint16_t)ulReloadValueForOneTick);

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
}

/*-----------------------------------------------------------*/

/* Override the default definition of vPortSuppressTicksAndSleep() that is
weakly defined in the FreeRTOS Cortex-M4 port layer with a version that manages
the LPTIM1 interrupt, as the tick is generated from LPTIM1 compare matches events. */

void vPortSuppressTicksAndSleep(TickType_t xExpectedIdleTime)
{
    uint32_t ulReloadValue, ulCompleteTickPeriods, ulCountBeforeSleep, ulCountAfterSleep, Compare_Value ;
    eSleepModeStatus eSleepAction;
    TickType_t xModifiableIdleTime;

    /* THIS FUNCTION IS CALLED WITH THE SCHEDULER SUSPENDED. */

    /* Make sure the TIMx reload value does not overflow the counter. */
    if(xExpectedIdleTime > xMaximumPossibleSuppressedTicks)
    {
        xExpectedIdleTime = xMaximumPossibleSuppressedTicks;
    }

    /* Calculate the reload value required to wait xExpectedIdleTime tick
    periods. */
    ulReloadValue = ulReloadValueForOneTick * xExpectedIdleTime;

    /* Stop TIMx momentarily.  The time TIMx is stopped for is not accounted for
    in this implementation (as it is in the generic implementation) because the
    clock is so slow it is unlikely to be stopped for a complete count period
    anyway. */
    do
    {
        ulCountBeforeSleep = HAL_LPTIM_ReadCounter(&hlptim1);
    }
    while(ulCountBeforeSleep != HAL_LPTIM_ReadCounter(&hlptim1));
    Compare_Value = hlptim1.Instance->CMP;
    HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);

    /* If this function is re-entered before one complete tick period then the
    reload value might be set to take into account a partial time slice, but
    just reading the count assumes it is counting up to a full ticks worth - so
    add in the difference if any. */
    ulCountBeforeSleep += (ulReloadValueForOneTick - Compare_Value);

    /* Enter a critical section but don't use the taskENTER_CRITICAL() method as
    that will mask interrupts that should exit sleep mode. */
    __asm volatile("cpsid i");
    __asm volatile("dsb");
    __asm volatile("isb");

    /* The tick flag is set to false before sleeping.  If it is true when sleep
    mode is exited then sleep mode was probably exited because the tick was
    suppressed for the entire xExpectedIdleTime period. */
    ulTickFlag = pdFALSE;

    /* If a context switch is pending then abandon the low power entry as
    the context switch might have been pended by an external interrupt that
    requires processing. */
    eSleepAction = eTaskConfirmSleepModeStatus();
    if(eSleepAction == eAbortSleep)
    {
        /* Restart tick. */
        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, (ulReloadValueForOneTick - ulCountBeforeSleep), (ulReloadValueForOneTick - ulCountBeforeSleep));

        /* Re-enable interrupts - see comments above the cpsid instruction()
        above. */
        __asm volatile("cpsie i");
    }
    else if(eSleepAction == eNoTasksWaitingTimeout)
    {
        /* A user definable macro that allows application code to be inserted
        	here.  Such application code can be used to minimise power consumption
        	further by turning off IO, peripheral clocks, the Flash, etc. */
        configPRE_SLEEP_PROCESSING(&xModifiableIdleTime);

        /* There are no running state tasks and no tasks that are blocked with a
        time out.  Assuming the application does not care if the tick time slips
        with respect to calendar time then enter a deep sleep that can only be
        woken by (in this demo case) the user button being pushed on the
        STM32L discovery board.  If the application does require the tick time
        to keep better track of the calender time then the RTC peripheral can be
        used to make rough adjustments. */
        HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

        /* A user definable macro that allows application code to be inserted
        here.  Such application code can be used to reverse any actions taken
        by the configPRE_STOP_PROCESSING().  In this demo
        configPOST_STOP_PROCESSING() is used to re-initialise the clocks that
        were turned off when STOP mode was entered. */
        configPOST_SLEEP_PROCESSING(&xModifiableIdleTime);

        /* Restart tick. */
        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValueForOneTick, ulReloadValueForOneTick);

        /* Re-enable interrupts - see comments above the cpsid instruction()
        above. */
        __asm volatile("cpsie i");
        __asm volatile("dsb");
        __asm volatile("isb");

    }
    else
    {
        /* Adjust the reload value to take into account that the current time
        slice is already partially complete. */
        ulReloadValue -= ulCountBeforeSleep;
        HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValue, ulReloadValue);

        /* Allow the application to define some pre-sleep processing.  This is
        the standard configPRE_SLEEP_PROCESSING() macro as described on the
        FreeRTOS.org website. */
        xModifiableIdleTime = xExpectedIdleTime;
        configPRE_SLEEP_PROCESSING(&xModifiableIdleTime);

        /* xExpectedIdleTime being set to 0 by configPRE_SLEEP_PROCESSING()
        means the application defined code has already executed the wait/sleep
        instruction. */
        if(xModifiableIdleTime > 0)
        {
            /* The sleep mode used is dependent on the expected idle time
            as the deeper the sleep the longer the wake up time.  See the
            comments at the top of main_low_power.c.  Note xRegulatorOffIdleTime
            is set purely for convenience of demonstration and is not intended
            to be an optimised value. */
            if(xModifiableIdleTime > (0.03 * configTICK_RATE_HZ)) // Running if greater than 30ms.
            {
                /* A slightly lower power sleep mode with a longer wake up
                time. */
                //HAL_PWREx_EnterSTOP0Mode(PWR_SLEEPENTRY_WFI);// 140uA idle current
                //HAL_PWREx_EnterSTOP1Mode(PWR_STOPENTRY_WFI); // 32uA idle current
                HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI); // 6.7uA idle current
            }
            else
            {

                /* A slightly higher power sleep mode with a faster wake up
                time. */
                HAL_PWREx_EnterSTOP0Mode(PWR_STOPENTRY_WFI);
            }
        }

        /* Allow the application to define some post sleep processing.  This is
        the standard configPOST_SLEEP_PROCESSING() macro, as described on the
        FreeRTOS.org website. */
        configPOST_SLEEP_PROCESSING(&xModifiableIdleTime);

        /* Stop TIMx.  Again, the time the clock is stopped for in not accounted
        for here (as it would normally be) because the clock is so slow it is
        unlikely it will be stopped for a complete count period anyway. */
        do
        {
            ulCountAfterSleep = HAL_LPTIM_ReadCounter(&hlptim1);
        }
        while(ulCountAfterSleep != HAL_LPTIM_ReadCounter(&hlptim1));
        HAL_LPTIM_TimeOut_Stop_IT(&hlptim1);

        /* Re-enable interrupts - see comments above the cpsid instruction()
        above. */
        __asm volatile("cpsie i");
        __asm volatile("dsb");
        __asm volatile("isb");

        if(ulTickFlag != pdFALSE)
        {
            /* The tick interrupt has already executed, although because this
            function is called with the scheduler suspended the actual tick
            processing will not occur until after this function has exited.
            Reset the reload value with whatever remains of this tick period. */
            ulReloadValue = ulReloadValueForOneTick - ulCountAfterSleep;

            HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValue, ulReloadValue);

            /* The tick interrupt handler will already have pended the tick
            processing in the kernel.  As the pending tick will be processed as
            soon as this function exits, the tick value	maintained by the tick
            is stepped forward by one less than the	time spent sleeping.  The
            actual stepping of the tick appears later in this function. */
            ulCompleteTickPeriods = xExpectedIdleTime - 1UL;
        }
        else
        {
            /* Something other than the tick interrupt ended the sleep.  How
            	many complete tick periods passed while the processor was
            	sleeping?  Add back in the adjustment that was made to the reload
            	value to account for the fact that a time slice was part way through
            	when this function was called. */
            ulCountAfterSleep += ulCountBeforeSleep;
            ulCompleteTickPeriods = ulCountAfterSleep / ulReloadValueForOneTick;

            /* The reload value is set to whatever fraction of a single tick
            period remains. */
            ulCountAfterSleep -= (ulCompleteTickPeriods * ulReloadValueForOneTick);
            ulReloadValue = ulReloadValueForOneTick - ulCountAfterSleep;

            if(ulReloadValue == 0)
            {
                /* There is no fraction remaining. */
                ulReloadValue = ulReloadValueForOneTick;
                ulCompleteTickPeriods++;
            }
            HAL_LPTIM_TimeOut_Start_IT(&hlptim1, ulReloadValue, ulReloadValue);
        }
        /* Restart TIMx so it runs up to the reload value.  The reload value
        will get set to the value required to generate exactly one tick period
        the next time the TIMx interrupt executes. */

        /* Wind the tick forward by the number of tick periods that the CPU
        remained in a low power state. */
        vTaskStepTick(ulCompleteTickPeriods);
    }
}
#endif



