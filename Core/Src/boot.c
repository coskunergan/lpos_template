/*
 *  Boot Librarys
 *
 *  Created on: Oct 12, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "boot.h"

FIRST_START_OS(Boot_Init);

static void BootTask(void *argument);

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Boot_Init(void)
{
    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "BootTask",
        .priority = (osPriority_t) osPriorityHigh,
        .stack_size = 256
    };
    osThreadNew(BootTask, NULL, &defaultTask_attributes);
}
/*********************************************************/
static void BootTask(void *argument)
{
#ifdef LIB_BUTTONS
#include "..\..\Libraries\lib_buttons\lib_buttons.h"
    SendConfigMsg_Buttons(eBUTTON_INIT, eBUTTON_ID_1, NULL);
    osDelay(100);
#endif

#ifdef LIB_LEDS
#include "..\..\Libraries\lib_leds\lib_leds.h"
    SendConfigMsg_Led(eLED_INIT, eLED_ID_1);
    SendConfigMsg_Led(eLED_INIT, eLED_ID_2);
    osDelay(100);
#endif

#ifdef LIB_BUZZER
#include "..\..\Libraries\lib_buzzer\lib_buzzer.h"
    SendConfigMsg_Buzzer(eBUZZER_INIT);
    SendDataMsg_Buzzer(BUZZER_TONE_A, BUZZER_SHORT_BEEP);
    osDelay(100);
#endif

#ifdef LIB_GLASSLCD
#include "..\..\Libraries\lib_glasslcd\lib_glasslcd.h"
    Glasslcd_Struct_t lcd;
    SendConfigMsg_Glasslcd(eGLASSLCD_INIT);
    lcd.command = eGLASSLCD_STRING;
    memset(lcd.string,8,13);
    SendDataMsg_Glasslcd(&lcd);
    lcd.command = eGLASSLCD_SET_ALL_ICON;
    SendDataMsg_Glasslcd(&lcd);		
    osDelay(100);
#endif

    osThreadTerminate(osThreadGetId());
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

