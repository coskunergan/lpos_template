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

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(3, sizeof(Led_Data_Frame_t) * 3, NULL);
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

/*********************************************************/
/*********************************************************/
/*********************************************************/

