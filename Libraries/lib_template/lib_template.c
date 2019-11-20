/*
 *  Template Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_template.h"

#ifdef LIB_TEMPLATE
FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  3
#define MSGQUEUE_OBJECT_SIZE sizeof(Template_Data_Frame_t)

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Template_Hw_Init();
extern void Template_Hw_DeInit();
extern void Template_Hw_Enable();
extern void Template_Hw_Disable();
extern void Template_Operation(Template_Data_Frame_t *data_msg);

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "TemplateTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        Error_Handler();
    }
}
/*********************************************************/
osStatus_t SendDataMsg_Template(Template_State_t state)
{
    Template_Data_Frame_t msg;

    msg.frame_type = eTEMPLATE_DATA_FRAME;
    msg.state = state;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendConfigMsg_Template(Template_Config_t config)
{
    Template_Config_Frame_t msg;

    msg.frame_type = eTEMPLATE_CONFIG_FRAME;
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
    Template_Data_Frame_t *data_msg;
    Template_Config_Frame_t *config_msg;
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, osWaitForever) == osOK)
        {
            if(*msg == eTEMPLATE_CONFIG_FRAME)
            {
                config_msg = (Template_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eTEMPLATE_INIT:
                        Template_Hw_Init();
                        break;
                    case eTEMPLATE_DEINIT:
                        Template_Hw_DeInit();
                        break;
                    case eTEMPLATE_ENABLE:
                        Template_Hw_Enable();
                        break;
                    case eTEMPLATE_DISABLE:
                        Template_Hw_Disable();
                        break;						
                    default:
                        break;
                }
            }
            else if(*msg == eTEMPLATE_DATA_FRAME)
            {
                data_msg = (Template_Data_Frame_t *)msg;
                Template_Operation(data_msg);
            }
        }
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
