/*
 *  Buzzer Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buzzer.h"

FIRST_START_OS(Lib_Init);

//__weak extern osStatus_t SendDataMsg_Buzzer(uint8_t tone, uint16_t active_time);

static osMessageQueueId_t mq_id;

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(3, sizeof(Buzzer_Data_Frame_t) * 3, NULL);
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

/*********************************************************/
/*********************************************************/
/*********************************************************/
