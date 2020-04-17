/*
 *  Buzzer Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_buzzer\lib_buzzer.h"

#ifdef LIB_BUZZER

/*********************************************************/
/*********************************************************/
/*********************************************************/
#ifdef LIB_BUTTONS
#include "..\Libraries\lib_buttons\lib_buttons.h"
static void Button_Blink_ISR(Button_State_t state)
{
    if(state == ePRESSED)
    {
        SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
    }

    if(state == eLONGPRESSED)
    {
        SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
        SendDataMsg_Buzzer(BUZZER_TONE_NONE, BUZZER_SHORT_BEEP);
        SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
    }
}
#endif
/*********************************************************/
void Func_Buzzer(void)
{
    //-------------------------------------
#ifdef LIB_BUTTONS
    SendConfigMsg_Buttons(eBUTTON_ADD_CALLBACK, eBUTTON_ID_1, Button_Blink_ISR);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
