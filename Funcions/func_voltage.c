/*
 *  Voltage Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_voltage\lib_voltage.h"

#ifdef LIB_VOLTAGE

extern GlobalStats_t GlobalStats;
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Func_Voltage(void)
{
    //-------------------------------------
#ifdef LIB_MODBUS
#include "..\..\Libraries\lib_modbus\lib_modbus.h"
    Modbus_Data_Frame_t modbus_msg =
    {
        .data = eMODBUS_ADD_RO_REG,
        .ptr = &GlobalStats.ReferanceVoltageLevelmV,
        .length = 1,
        .addres = 1003,
    };
    SendDataMsg_Modbus(&modbus_msg);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
