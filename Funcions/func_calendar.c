/*
 *  Calendar Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_calendar\lib_calendar.h"

#ifdef LIB_CALENDAR

extern struct calendar_date date;
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Func_Calendar(void)
{
    //---- Scheduler First Init Delay -----
    osSemaphoreId_t InitTime_Sem = osSemaphoreNew(1, 0, NULL);
    osSemaphoreAcquire(InitTime_Sem, 40);
    osSemaphoreDelete(InitTime_Sem);
    //-------------------------------------
    SendConfigMsg_Calendar(eCALENDAR_INIT, NULL, NULL);
    static struct calendar_date date =
    {
        .second = 0,
        .minute = 31,
        .hour = 17,
        .date = 5,  //6.day
        .month = 04, //April
        .year = 2020
    };
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
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
