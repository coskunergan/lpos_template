/*
 *  Boot Librarys
 *
 *  Created on: Oct 12, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "sys_boot.h"

FIRST_START_OS(Boot_Init);

static void BootTask(void *argument);

static osMutexId_t Mutex_Adc = NULL;
static osMutexId_t Mutex_Uart = NULL;
GlobalStats_t GlobalStats;

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Boot_Init(void)
{
    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "BootTask",
        .priority = (osPriority_t) osPriorityHigh,
        .stack_size = 512
    };
    osThreadNew(BootTask, NULL, &defaultTask_attributes);
}
/*********************************************************/
osMutexId_t Get_Hw_Mutex(Hw_Mutex_t mt)
{
    switch(mt)
    {
        case MUTEX_ADC:
            if(Mutex_Adc == NULL)
            {
                Mutex_Adc = osMutexNew(NULL);
            }
            return Mutex_Adc;
        case MUTEX_UART:
            if(Mutex_Uart == NULL)
            {
                Mutex_Uart = osMutexNew(NULL);
            }
            return Mutex_Uart;
        default:
            break;
    }
    return NULL;
}
/*********************************************************/
static void BootTask(void *argument)
{
    SEGGER_SYSVIEW_Start();

    DBG_PRINTF("Boot started!");

    osThreadTerminate(osThreadGetId());
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

