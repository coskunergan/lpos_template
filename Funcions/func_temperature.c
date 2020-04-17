/*
 *  Temperature Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_temperature\lib_temperature.h"

#ifdef LIB_TEMPERATURE

extern GlobalStats_t GlobalStats;
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Func_Temperature(void)
{
    //-------------------------------------
#ifdef LIB_MODBUS
#include "..\..\Libraries\lib_modbus\lib_modbus.h"
    Modbus_Data_Frame_t modbus_msg =
    {
        .data = eMODBUS_ADD_RO_REG,
        .ptr = &GlobalStats.TemperatureLevelCelsius,
        .length = 1,
        .addres = 1004,
    };
    SendDataMsg_Modbus(&modbus_msg);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif

