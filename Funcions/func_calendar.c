/*
 *  Calendar Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_calendar\lib_calendar.h"

#ifdef LIB_CALENDAR

static struct calendar_date date;
/*********************************************************/
/*********************************************************/
/*********************************************************/
#ifdef LIB_LORAWAN
#include "..\..\Libraries\lib_lorawan\lib_lorawan.h"
static void LoraTimeUpdate(uint32_t timestamp)
{
    calendar_timestamp_to_date(timestamp, &date);
    SendConfigMsg_Calendar(eCALENDAR_SETTIME, &date, NULL);
}
#endif
/*********************************************************/
void Func_Calendar(void)
{
    //---- Scheduler First Init Delay -----
    osSemaphoreId_t InitTime_Sem = osSemaphoreNew(1, 0, NULL);
    osSemaphoreAcquire(InitTime_Sem, 40);
    osSemaphoreDelete(InitTime_Sem);
    //-------------------------------------
    SendConfigMsg_Calendar(eCALENDAR_INIT, NULL, NULL);
    date.second = 0;
    date.minute = 50;
    date.hour = 9;
    date.date = 24; //25.day
    date.month = 6; //July
    date.year = 2020;
    SendConfigMsg_Calendar(eCALENDAR_SETTIME, &date, NULL);
    DBG_PRINTF("Calendar lib boot done!");
    //-------------------------------------
#ifdef LIB_MODBUS
#include "..\..\Libraries\lib_modbus\lib_modbus.h"
    Modbus_Data_Frame_t modbus_msg =
    {
        .data = eMODBUS_ADD_RW_REG,
        .ptr = (void *) &date.second,
        .length = 4, // 8 byte
        .addres = 1,
    };
    SendDataMsg_Modbus(&modbus_msg);
#endif
    //-------------------------------------
#ifdef LIB_LORAWAN
    Lorawan_Config_Frame_t msg;
    msg.state = eLORAWAN_ADD_CALLBACK;
    msg.Callback_Type = eLORAWAN_TIMEUPDATE_CB;
    msg.vfPtr = LoraTimeUpdate;
    SendConfigMsg_Lorawan(&msg);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
