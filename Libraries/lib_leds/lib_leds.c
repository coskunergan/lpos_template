/*
 *  Leds Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_leds.h"

FIRST_START_OS(Lib_Init);

//__weak osStatus_t SendDataMsg_Led(Led_ID_t led_id, Led_State_t state);

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Leds_Hw_Init(void);
extern void Leds_Hw_DeInit(void);
extern void Led_Operation(Led_Data_Frame_t *data_msg);

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(3, sizeof(Led_Data_Frame_t) * 3, NULL);

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

    msg.frame_type = eLEDS_DATA_FRAME;
    msg.led_id = led_id;
    msg.state = state;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 100);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendConfigMsg_Led(Led_Config_t config)
{
    Led_Config_Frame_t msg;

    msg.frame_type = eLEDS_CONFIG_FRAME;
    msg.config = config;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 100);
    }
    return osErrorNoMemory;
}
/*********************************************************/
static void StartTask(void *argument)
{
    Led_Data_Frame_t *data_msg;
    Led_Config_Frame_t *config_msg;
    uint8_t *msg = malloc(osMessageQueueGetMsgSize(mq_id));

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, 500) == osOK)
        {
            if(*msg == eLEDS_CONFIG_FRAME)
            {
                config_msg = (Led_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eLEDS_CONFIG_INIT:
                        Leds_Hw_Init();
                        break;
                    case eLEDS_CONFIG_DEINIT:
                        Leds_Hw_DeInit();
                        break;
                    default:
                        break;
                }
            }
            else if(*msg == eLEDS_DATA_FRAME)
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

