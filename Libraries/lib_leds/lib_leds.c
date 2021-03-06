/*
 *  Leds Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_leds.h"

#ifdef LIB_LEDS
FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  8

static uint8_t msg[LED_MSGQUEUE_OBJECT_SIZE];

static osMessageQueueId_t mq_id;

uint16_t leds_state;

static void StartTask(void *argument);

extern void Leds_Hw_Init(Led_ID_t led_id);
extern void Leds_Hw_DeInit(Led_ID_t led_id);
extern void Led_Operation(Led_Data_Frame_t *data_msg);
extern void Func_Leds(void);
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, LED_MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "LedsTask",
        .priority = (osPriority_t) osPriorityBelowNormal,
        .stack_size = 256
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        vAssertCalled(__FILE__, __LINE__);
    }
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
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
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
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
static void StartTask(void *argument)
{
    Led_Data_Frame_t *data_msg;
    Led_Config_Frame_t *config_msg;    

    Func_Leds();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, osWaitForever) == osOK)
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
                switch(data_msg->state)
                {
                    case eLED_ON:
                        DBG_PRINTF("Led %d ON", data_msg->led_id);
                        leds_state |= (1UL << data_msg->led_id);
                        break;
                    case eLED_OFF:
                        DBG_PRINTF("Led %d OFF", data_msg->led_id);
                        leds_state &= ~(1UL << data_msg->led_id);
                        break;
                    case eLED_TOGGLE:
                        DBG_PRINTF("Led %d TOGGLE", data_msg->led_id);
                        leds_state ^= (1UL << data_msg->led_id);
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

