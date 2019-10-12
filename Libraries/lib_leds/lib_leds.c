/*
 *  Leds Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_leds.h"

FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  5
#define MSGQUEUE_OBJECT_SIZE sizeof(Led_Data_Frame_t)

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Leds_Hw_Init(Led_ID_t led_id);
extern void Leds_Hw_DeInit(Led_ID_t led_id);
extern void Led_Operation(Led_Data_Frame_t *data_msg);

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "LedsTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128
    };
    osThreadNew(StartTask, NULL, &defaultTask_attributes);

}
/*********************************************************/
osStatus_t SendDataMsg_Led(Led_ID_t led_id, Led_State_t state)
{
    Led_Data_Frame_t msg;

    msg.frame_type = eLED_DATA_FRAME;
    msg.led_id = led_id;
    msg.state = state;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 100);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendConfigMsg_Led(Led_Config_t config, Led_ID_t led_id)
{
    Led_Config_Frame_t msg;

    msg.frame_type = eLED_CONFIG_FRAME;
    msg.config = config;
    msg.led_id = led_id;

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
        SendDataMsg_Led(eLED_ID_1, eLED_ON);
    }
    else
    {
        SendDataMsg_Led(eLED_ID_1, eLED_OFF);
    }
}
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
    Led_Data_Frame_t *data_msg;
    Led_Config_Frame_t *config_msg;
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];

    Button_Procces();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, 500) == osOK)
        {
            if(*msg == eLED_CONFIG_FRAME)
            {
                config_msg = (Led_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eLED_INIT:
                        Leds_Hw_Init(config_msg->led_id);
                        break;
                    case eLED_DEINIT:
                        Leds_Hw_DeInit(config_msg->led_id);
                        break;
                    default:
                        break;
                }
            }
            else if(*msg == eLED_DATA_FRAME)
            {
                data_msg = (Led_Data_Frame_t *)msg;
                Led_Operation(data_msg);
            }
        }
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

