/*
 *  Buzzer Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buzzer.h"

FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  5
#define MSGQUEUE_OBJECT_SIZE sizeof(Buzzer_Data_Frame_t)

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Buzzer_Hw_Init(void);
extern void Buzzer_Hw_DeInit(void);
extern void Buzzer_Operation(Buzzer_State_t state);

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "BuzzerTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128
    };
    osThreadNew(StartTask, NULL, &defaultTask_attributes);
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
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 100);
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
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 100);
    }
    return osErrorNoMemory;
}
/*********************************************************/
#ifdef LIB_BUTTONS
#include "..\lib_buttons\lib_buttons.h"
//#include "..\..\Libraries\lib_buttons\lib_buttons.h"
static void Button_Blink_ISR(Button_State_t *state)
{
    if(*state == ePRESSED)
    {
        SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
    }

    if(*state == eLONGPRESSED)
    {
        SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
        SendDataMsg_Buzzer(BUZZER_TONE_NONE, BUZZER_SHORT_BEEP);
        SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
    }
}
/*********************************************************/
static void Button_Procces(void)
{
    SendConfigMsg_Buttons(eBUTTON_ADD_CALLBACK, eBUTTON_ID_1, Button_Blink_ISR);
}
#else
__weak void Button_Procces(void);
#endif
/*********************************************************/
static void StartTask(void *argument)
{
    Buzzer_Data_Frame_t *data_msg;
    Buzzer_Config_Frame_t *config_msg;
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];

    Button_Procces();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, 500) == osOK)
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
                    osDelay(data_msg->active_time);
                    Buzzer_Operation(eBUZZER_OFF);
                }
                if(data_msg->tone == 255)// buzzer ignore tone
                {
                    Buzzer_Operation(eBUZZER_OFF);
                    osDelay(data_msg->active_time);// buzzer ignore tone
                }
                else
                {
                    for(int i = 0; i < data_msg->tone; i++)// buzzer frequence tone
                    {
                        Buzzer_Operation(eBUZZER_TOGGLE);
                        osDelay(data_msg->active_time / data_msg->tone);
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
