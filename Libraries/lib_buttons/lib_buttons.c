/*
 *  Buttons Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buttons.h"

#ifdef LIB_BUTTONS
FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  16
#define	BUTTON_CALLBACK_LIMIT 32
#define BUTTON_LONGPRESS_PERIOD_TICK  3000

static osMessageQueueId_t mq_id;

uint16_t buttons_state;

static void StartTask(void *argument);

extern void Buttons_Hw_DeInit(Button_ID_t button_id);
extern void Buttons_Hw_Init(Button_ID_t button_id);
extern void Buttons_Hw_Enable(Button_ID_t button_id);
extern void Buttons_Hw_Disable(Button_ID_t button_id);
extern void Func_Buttons(void);

Button_Cb_List_t Button_CbFunc_List[eBUTTON_ID_NUMBEROFTYPE];
static struct button_cb_list_t Button_Func_Handle[BUTTON_CALLBACK_LIMIT];
static osTimerId_t Timer_ID[eBUTTON_ID_NUMBEROFTYPE];
static uint32_t Timer_Arg[eBUTTON_ID_NUMBEROFTYPE];

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, BUTTONS_MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "ButtonsTask",
        .priority = (osPriority_t) osPriorityAboveNormal,
        .stack_size = 256
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        Error_Handler();
    }
}
/*********************************************************/
osStatus_t SendDataMsg_Buttons(Button_Data_t data, Button_ID_t button_id)
{
    Buttons_Data_Frame_t msg;

    msg.frame_type = eBUTTON_DATA_FRAME;
    msg.data = data;
    msg.button_id = button_id;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
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
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
void Timers_Callback(void *arg)
{
    DBG_PRINTF("Button %d Long Pressed", *((uint32_t *)(arg)));
    BUTTON_CALLBACK_FUNC(*((uint32_t *)(arg)), eLONGPRESSED);
}
/*********************************************************/
static void StartTask(void *argument)
{
    Buttons_Config_Frame_t *config_msg;
    Buttons_Data_Frame_t *data_msg;
    uint8_t msg[BUTTONS_MSGQUEUE_OBJECT_SIZE];
    uint8_t i;

    Func_Buttons();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, osWaitForever) == osOK)
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
            else if(*msg == eBUTTON_DATA_FRAME)
            {
                data_msg = (Buttons_Data_Frame_t *)msg;
                switch(data_msg->data)
                {
                    case eBUTTON_TIMER_START:
                        if(Timer_ID[data_msg->button_id] == NULL)
                        {
                            Timer_Arg[data_msg->button_id] = data_msg->button_id;
                            Timer_ID[data_msg->button_id] = osTimerNew(Timers_Callback, osTimerOnce, &Timer_Arg[data_msg->button_id], NULL);//create timer
                            osTimerStart(Timer_ID[data_msg->button_id], BUTTON_LONGPRESS_PERIOD_TICK);
                            DBG_PRINTF("Button %d Pressed", data_msg->button_id);
                        }
                        break;
                    case	eBUTTON_TIMER_STOP:
                        if(Timer_ID[data_msg->button_id] != NULL)
                        {
                            osTimerDelete(Timer_ID[data_msg->button_id]); //destroy timer
                            Timer_ID[data_msg->button_id] = NULL;
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
#endif

