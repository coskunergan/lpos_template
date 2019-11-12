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
#endif

#define MSGQUEUE_OBJECTS  5
#define MSGQUEUE_OBJECT_SIZE sizeof(Glasslcd_Data_Frame_t)

static osMessageQueueId_t mq_id;

static void StartTask(void *argument);

extern void Glasslcd_IconChange(uint8_t icon , uint8_t state);
extern void Glasslcd_IconChangeAll(uint8_t state);
extern void Glasslcd_WriteStruct(Glasslcd_Struct_t *lcd);
extern void Glasslcd_Hw_Init(void);
extern void Glasslcd_Hw_DeInit(void);
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "GlassLcdTask",
        .priority = (osPriority_t) osPriorityNormal,
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
#ifdef LIB_CALENDAR
#include "..\..\Libraries\lib_calendar\lib_calendar.h"
extern struct calendar_date date;
extern GlobalStats_t GlobalStats;
static void Calendar_ISR(Calendar_Passed_t passed)
{
    char equal_char;
    static uint8_t per = 0;
    static bool show_batt = 0;

    Glasslcd_Struct_t lcd;
    if(passed == eSECOND_PASSED)
    {
        if(per)
        {
            equal_char = ' ';
            per = 0;
        }
        else
        {
            equal_char = '=';
            per = 1;
        }
        if((date.second % 10) == 0)
        {
            show_batt = !show_batt;
        }
        if(show_batt)
        {
            sprintf(lcd.string, "%02d%c%02d %02d %04d", date.hour, equal_char, date.minute, date.second, GlobalStats.ReferanceVoltageLevelmV);
        }
        else
        {
            sprintf(lcd.string, "%02d%c%02d %02d % 02d'", date.hour, equal_char, date.minute, date.second, GlobalStats.TemperatureLevelCelsius - 15);
        }
        lcd.command = eGLASSLCD_STRING;
        SendDataMsg_Glasslcd(&lcd);
    }
}
static void Clock_Demo(void)
{
    SendConfigMsg_Calendar(eCALENDAR_ADD_CALLBACK, NULL, Calendar_ISR);
}
#else
__weak void Clock_Demo(void);
#endif
/*********************************************************/
static void StartTask(void *argument)
{
    Glasslcd_Data_Frame_t *data_msg;
    Glasslcd_Config_Frame_t *config_msg;
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];

    Clock_Demo();

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
