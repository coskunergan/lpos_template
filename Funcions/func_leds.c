/*
 *  Leds Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_leds\lib_leds.h"

#ifdef LIB_LEDS

extern uint16_t leds_state;
/*********************************************************/
/*********************************************************/
/*********************************************************/
#ifdef LIB_BUTTONS
#include "..\Libraries\lib_buttons\lib_buttons.h"
static void Button_Blink_ISR(Button_State_t state)
{
    if(state == ePRESSED)
    {
        SendDataMsg_Led(eLED_ID_1, eLED_ON);
    }
    else if(state == eLONGPRESSED)
    {
        SendDataMsg_Led(eLED_ID_1, eLED_OFF);
        SendDataMsg_Led(eLED_ID_2, eLED_ON);
    }
    else
    {
        SendDataMsg_Led(eLED_ID_1, eLED_OFF);
        SendDataMsg_Led(eLED_ID_2, eLED_OFF);
    }
}
#endif
/*********************************************************/
void Func_Leds(void)
{
    //-------------------------------------
#ifdef LIB_BUTTONS
    SendConfigMsg_Buttons(eBUTTON_ADD_CALLBACK, eBUTTON_ID_1, Button_Blink_ISR);
#endif
    //-------------------------------------
#ifdef LIB_MODBUS
#include "..\..\Libraries\lib_modbus\lib_modbus.h"
    Modbus_Data_Frame_t modbus_msg =
    {
        .data = eMODBUS_ADD_RO_REG,
        .ptr = &leds_state,
        .length = 1,
        .addres = 1001,
    };
    SendDataMsg_Modbus(&modbus_msg);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
