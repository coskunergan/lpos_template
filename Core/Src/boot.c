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

GlobalStats_t GlobalStats;

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

#ifdef LIB_CALENDAR
#include "..\..\Libraries\lib_calendar\lib_calendar.h"
    SendConfigMsg_Calendar(eCALENDAR_INIT, NULL, NULL);
    struct calendar_date date =
    {
        .second = 45,
        .minute = 15,
        .hour = 17,
        .date = 27,  //25.day
        .month = 10, //November
        .year = 2019
    };
    SendConfigMsg_Calendar(eCALENDAR_SETTIME, &date, NULL);
    osDelay(100);
#endif

#ifdef LIB_GLASSLCD
#include "..\..\Libraries\lib_glasslcd\lib_glasslcd.h"
    Glasslcd_Struct_t lcd;
    SendConfigMsg_Glasslcd(eGLASSLCD_INIT);
    lcd.command = eGLASSLCD_STRING;
    memset(lcd.string, '8', 13);
    SendDataMsg_Glasslcd(&lcd);
    osDelay(100);
#endif

#if defined(LIB_VOLTAGE) || defined(LIB_TEMPERATURE)
    static osMutexId_t ADC1_Mutex = NULL;
    ADC1_Mutex = osMutexNew(NULL);
#endif

#ifdef LIB_VOLTAGE
#include "..\..\Libraries\lib_voltage\lib_voltage.h"
    SendConfigMsg_Voltage(eVOLTAGE_INIT, ADC1_Mutex, 10000);
    osDelay(100);
#endif

#ifdef LIB_TEMPERATURE
#include "..\..\Libraries\lib_temperature\lib_temperature.h"
    SendConfigMsg_Temperature(eTEMPERATURE_INIT, ADC1_Mutex, 10000);
    osDelay(100);
#endif

#ifdef LIB_MODBUS
#include "..\..\Libraries\lib_modbus\lib_modbus.h"
    Modbus_Config_Frame_t msg =
    {
        .config = eMODBUS_INIT,
        .baudrate = 9600
    };
    SendConfigMsg_Modbus(&msg);
    osDelay(100);
#endif

    osThreadTerminate(osThreadGetId());
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

