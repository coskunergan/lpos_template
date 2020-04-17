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
