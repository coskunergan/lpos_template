/*
 *  Buttons Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buttons.h"

FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  5
#define MSGQUEUE_OBJECT_SIZE sizeof(Buttons_Config_Frame_t)
#define	BUTTON_CALLBACK_LIMIT 32

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Buttons_Hw_DeInit(Button_ID_t button_id);
extern void Buttons_Hw_Init(Button_ID_t button_id);
extern void Buttons_Hw_Enable(Button_ID_t button_id);
extern void Buttons_Hw_Disable(Button_ID_t button_id);

Button_Cb_List_t Button_CbFunc_List[eBUTTON_ID_NUMBEROFTYPE];
struct button_cb_list_t Button_Func_Handle[BUTTON_CALLBACK_LIMIT];

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "ButtonsTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 128
    };
    osThreadNew(StartTask, NULL, &defaultTask_attributes);
}
/*********************************************************/
osStatus_t SendConfigMsg_Buttons(Button_Config_t config, Button_ID_t button_id, void *cb_func)
{
    Buttons_Config_Frame_t msg;

    msg.frame_type = eBUTTON_CONFIG_FRAME;
    msg.config = config;
    msg.button_id = button_id;
    msg.vfPtr = (void	(*)(Button_State_t state))cb_func;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 100);
    }
    return osErrorNoMemory;
}
/*********************************************************/
static void StartTask(void *argument)
{
    Buttons_Config_Frame_t *config_msg;
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];
    uint8_t i;

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, 500) == osOK)
        {
            if(*msg == eBUTTON_CONFIG_FRAME)
            {
                config_msg = (Buttons_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eBUTTON_INIT:
                        Buttons_Hw_Init(config_msg->button_id);
                        break;
                    case	eBUTTON_DEINIT:
                        Buttons_Hw_DeInit(config_msg->button_id);
                        break;
                    case eBUTTON_DISABLE:
                        Buttons_Hw_Disable(config_msg->button_id);
                        break;
                    case eBUTTON_ENABLE:
                        Buttons_Hw_Enable(config_msg->button_id);
                        break;
                    case eBUTTON_ADD_CALLBACK:
                        for(i = 0; i < BUTTON_CALLBACK_LIMIT; i++)// find
                        {
                            if(Button_Func_Handle[i].vfPtr == NULL)
                            {
                                break;
                            }
                        }
                        if(i != BUTTON_CALLBACK_LIMIT)
                        {
                            if(SysList_Length((list_t)&Button_CbFunc_List[config_msg->button_id]) == 0)
                            {
                                SysList_Init((list_t)&Button_CbFunc_List[config_msg->button_id]);
                            }
                            Button_Func_Handle[i].vfPtr = (void	(*)(Button_State_t state))config_msg->vfPtr;
                            SysList_Add((list_t)&Button_CbFunc_List[config_msg->button_id], &Button_Func_Handle[i]);// append
                        }
                        break;
                    case eBUTTON_DEL_CALLBACK:
                        for(i = 0; i < BUTTON_CALLBACK_LIMIT; i++)// find
                        {
                            if(Button_Func_Handle[i].vfPtr == config_msg->vfPtr)
                            {
                                break;
                            }
                        }
                        if(i != BUTTON_CALLBACK_LIMIT)
                        {
                            Button_Func_Handle[i].vfPtr = NULL;
                            SysList_Remove((list_t)&Button_CbFunc_List[config_msg->button_id], &Button_Func_Handle[i]);//destroy
                        }
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
