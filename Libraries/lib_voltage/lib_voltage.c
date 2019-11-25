/*
 *  Voltage Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_voltage.h"

#ifdef LIB_VOLTAGE
FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  3
#define MSGQUEUE_OBJECT_SIZE sizeof(Voltage_Config_Frame_t)

static osMessageQueueId_t mq_id;
static osTimerId_t Timer_ID;
static osMutexId_t *Hw_Mutex = NULL;

extern GlobalStats_t GlobalStats;

static void StartTask(void *argument);
extern void Voltage_Hw_Init(void);
extern void Voltage_Hw_DeInit(void);
extern uint32_t Voltage_Read(void);
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "VoltageTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 384
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        Error_Handler();
    }
}
/*********************************************************/
osStatus_t SendConfigMsg_Voltage(Voltage_Config_t config, osMutexId_t hw_mutex,  uint32_t period_ms)
{
    Voltage_Config_Frame_t msg;

    msg.frame_type = eVOLTAGE_CONFIG_FRAME;
    msg.config = config;
    msg.period_ms = period_ms;
    msg.hw_mutex = hw_mutex;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendDataMsg_Voltage(Voltage_Data_t data)
{
    Voltage_Data_Frame_t msg;

    msg.frame_type = eVOLTAGE_DATA_FRAME;
    msg.data = data;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
static void Timer_Callback(void *arg)
{
    SendDataMsg_Voltage(eVOLTAGE_REFRESH);
}
/*********************************************************/
static void StartTask(void *argument)
{
    Voltage_Config_Frame_t *config_msg;
    Voltage_Data_Frame_t *data_msg;
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, osWaitForever) == osOK)
        {
            if(*msg == eVOLTAGE_CONFIG_FRAME)
            {
                config_msg = (Voltage_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eVOLTAGE_INIT:
                        Hw_Mutex = config_msg->hw_mutex;
                        osMutexAcquire(*Hw_Mutex, osWaitForever);
                        Voltage_Hw_Init();
                        GlobalStats.ReferanceVoltageLevelmV = Voltage_Read();
                        osMutexRelease(*Hw_Mutex);
                        Timer_ID = osTimerNew(Timer_Callback, osTimerPeriodic, NULL, NULL); //create timer
                        osTimerStart(Timer_ID, config_msg->period_ms);
                        break;
                    case eVOLTAGE_DEINIT:
                        osMutexAcquire(*Hw_Mutex, osWaitForever);
                        Voltage_Hw_DeInit();
                        osMutexRelease(*Hw_Mutex);
                        osTimerDelete(Timer_ID); //destroy timer
                        break;
                    case eVOLTAGE_ENABLE:
                        osTimerStart(Timer_ID, NULL);
                        break;
                    case eVOLTAGE_DISABLE:
                        osTimerStop(Timer_ID);
                        break;
                    default:
                        break;
                }
            }
            else if(*msg == eVOLTAGE_DATA_FRAME)
            {
                data_msg = (Voltage_Data_Frame_t *)msg;
                switch(data_msg->data)
                {
                    case eVOLTAGE_REFRESH:
                        osMutexAcquire(*Hw_Mutex, osWaitForever);
                        GlobalStats.ReferanceVoltageLevelmV = Voltage_Read();
                        osMutexRelease(*Hw_Mutex);
                        break;
                    default:
                        break;
                }
            }
        }
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
