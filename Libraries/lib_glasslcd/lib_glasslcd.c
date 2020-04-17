/*
 *  Glass Lcd Library
 *
 *  Created on: Oct 22, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_glasslcd.h"

#ifdef LIB_GLASSLCD
FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  5

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Glasslcd_IconChange(uint8_t icon , uint8_t state);
extern void Glasslcd_IconChangeAll(uint8_t state);
extern void Glasslcd_WriteStruct(Glasslcd_Struct_t *lcd);
extern void Glasslcd_Hw_Init(void);
extern void Glasslcd_Hw_DeInit(void);
extern void Func_Glasslcd(void);
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, GLASSLCD_MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "GlassLcdTask",
        .priority = (osPriority_t) osPriorityBelowNormal,
        .stack_size = 256
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        Error_Handler();
    }
}
/*********************************************************/
osStatus_t SendDataMsg_Glasslcd(Glasslcd_Struct_t *lcd)
{
    Glasslcd_Data_Frame_t msg;

    msg.frame_type = eGLASSLCD_DATA_FRAME;
    msg.lcd = *lcd;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendConfigMsg_Glasslcd(Glasslcd_Config_t config)
{
    Glasslcd_Config_Frame_t msg;

    msg.frame_type = eGLASSLCD_CONFIG_FRAME;
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
    Glasslcd_Data_Frame_t *data_msg;
    Glasslcd_Config_Frame_t *config_msg;
    uint8_t msg[GLASSLCD_MSGQUEUE_OBJECT_SIZE];

    Func_Glasslcd();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, osWaitForever) == osOK)
        {
            if(*msg == eGLASSLCD_CONFIG_FRAME)
            {
                config_msg = (Glasslcd_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eGLASSLCD_INIT:
                        Glasslcd_Hw_Init();
                        break;
                    case eGLASSLCD_DEINIT:
                        Glasslcd_Hw_DeInit();
                        break;
                    default:
                        break;
                }
            }
            else if(*msg == eGLASSLCD_DATA_FRAME)
            {
                data_msg = (Glasslcd_Data_Frame_t *)msg;
                Glasslcd_WriteStruct(&data_msg->lcd);
            }
        }
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
