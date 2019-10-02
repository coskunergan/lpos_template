/*
 *  Buttons Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buttons.h"

FIRST_START_OS(Lib_Init);

//__weak osStatus_t SendConfigMsg_Buttons(Buttons_Config_t config, Button_ID_t button_id, void *cb_func);

static osMessageQueueId_t mq_id;
Button_List_t Button_CbFunc_List[eBUTTON_ID_NUMBEROFTYPE];

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(3, sizeof(Buttons_Config_Frame_t) * 3, NULL);
}
/*********************************************************/
osStatus_t SendConfigMsg_Buttons(Buttons_Config_t config, Button_ID_t button_id, void *cb_func)
{
    Buttons_Config_Frame_t msg;
	
    msg.frame_type = eBUTTONS_CONFIG_FRAME;
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

//	  if(SysList_Length((list_t)&Button_CbFunc_List[button_id]) == 0)
//    {
//        SysList_Init((list_t)&Button_CbFunc_List[button_id]);
//    }
//    isr_handle->vfPtr = (void	(*)(Button_State_t state))cb_func;
//    SysList_Add((list_t)&Button_CbFunc_List[button_id], isr_handle);	

/*********************************************************/
/*********************************************************/
/*********************************************************/
