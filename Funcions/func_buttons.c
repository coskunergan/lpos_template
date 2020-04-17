/*
 *  Buttons Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_buttons\lib_buttons.h"

#ifdef LIB_BUTTONS

extern uint16_t buttons_state;
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Func_Buttons(void)
{
    //-------------------------------------
#ifdef LIB_MODBUS
#include "..\..\Libraries\lib_modbus\lib_modbus.h"
    Modbus_Data_Frame_t modbus_msg =
    {
        .data = eMODBUS_ADD_RO_REG,
        .ptr = &buttons_state,
        .length = 1,
        .addres = 1002,
    };
    SendDataMsg_Modbus(&modbus_msg);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
