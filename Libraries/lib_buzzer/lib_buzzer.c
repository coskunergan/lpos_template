/*
 *  Buzzer Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buzzer.h"

#ifdef LIB_BUZZER
FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  8

static uint8_t msg[BUZZER_MSGQUEUE_OBJECT_SIZE];

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Buzzer_Hw_Init(void);
extern void Buzzer_Hw_DeInit(void);
extern void Buzzer_Operation(Buzzer_State_t state);
extern void Func_Buzzer(void);
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, BUZZER_MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "BuzzerTask",
        .priority = (osPriority_t) osPriorityBelowNormal,
        .stack_size = 256
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        vAssertCalled(__FILE__, __LINE__);
    }
}
/*********************************************************/
osStatus_t SendDataMsg_Buzzer(uint8_t tone, uint16_t active_time)
{
    Buzzer_Data_Frame_t msg;

    msg.frame_type = eBUZZER_DATA_FRAME;
    msg.tone = tone;
    msg.active_time = active_time;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendConfigMsg_Buzzer(Buzzer_Config_t config)
{
    Buzzer_Config_Frame_t msg;

    msg.frame_type = eBUZZER_CONFIG_FRAME;
    msg.config = config;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
static void StartTask(void *argument)
{
    Buzzer_Data_Frame_t *data_msg;
    Buzzer_Config_Frame_t *config_msg;
    osSemaphoreId_t Buzzer_Sem = osSemaphoreNew(1, 0, NULL);

    Func_Buzzer();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, osWaitForever) == osOK)
        {
            if(*msg == eBUZZER_CONFIG_FRAME)
            {
                config_msg = (Buzzer_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eBUZZER_INIT:
                        Buzzer_Hw_Init();
                        break;
                    case eBUZZER_DEINIT:
                        Buzzer_Hw_DeInit();
                        break;
                    default:
                        break;
                }
            }
            else if(*msg == eBUZZER_DATA_FRAME)
            {
                data_msg = (Buzzer_Data_Frame_t *)msg;
                if(data_msg->tone == 0)// buzzer active tone
                {
                    Buzzer_Operation(eBUZZER_ON);
                    DBG_PRINTF("Buzzer Beep %d ms", data_msg->active_time);

                    osSemaphoreAcquire(Buzzer_Sem, data_msg->active_time);
                    Buzzer_Operation(eBUZZER_OFF);
                }
                else if(data_msg->tone == 255)// buzzer ignore tone
                {
                    Buzzer_Operation(eBUZZER_OFF);
                    osSemaphoreAcquire(Buzzer_Sem, data_msg->active_time);	// buzzer ignore tone
                }
                else
                {
                    DBG_PRINTF("Buzzer Tone %d", data_msg->tone);
                    for(int i = 0; i < data_msg->tone; i++)// buzzer frequence tone
                    {
                        Buzzer_Operation(eBUZZER_TOGGLE);
                        osSemaphoreAcquire(Buzzer_Sem, data_msg->active_time / data_msg->tone);
                    }
                    Buzzer_Operation(eBUZZER_OFF);
                }
            }
        }
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
