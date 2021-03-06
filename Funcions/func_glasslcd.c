/*
 *  Glasslcd Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_glasslcd\lib_glasslcd.h"

#ifdef LIB_GLASSLCD

/*********************************************************/
/*********************************************************/
/*********************************************************/
#ifdef LIB_CALENDAR
#include "..\..\Libraries\lib_calendar\lib_calendar.h"
extern struct calendar_date date;
extern GlobalStats_t GlobalStats;
static void Calendar_ISR(Calendar_Passed_t passed)
{
    char equal_char;
    static uint8_t per = 0;
    static bool show_batt = 0;

    Glasslcd_Struct_t lcd;
    if(passed == eSECOND_PASSED)
    {
        if(per)
        {
            equal_char = ' ';
            per = 0;
        }
        else
        {
            equal_char = '=';
            per = 1;
        }
        if((date.second % 10) == 0)
        {
            show_batt = !show_batt;
        }
        if(show_batt)
        {
            sprintf(lcd.string, "%02d%c%02d %02d %04d", date.hour, equal_char, date.minute, date.second, GlobalStats.ReferanceVoltageLevelmV);
        }
        else
        {
            sprintf(lcd.string, "%02d%c%02d %02d % 02d'", date.hour, equal_char, date.minute, date.second, GlobalStats.TemperatureLevelCelsius - 15);
        }
        lcd.command = eGLASSLCD_STRING;
        SendDataMsg_Glasslcd(&lcd);
    }
}
#endif
/*********************************************************/
void Alert_Message(const char *pcFile, unsigned long ulLine)
{
    extern void Glasslcd_WriteStruct(Glasslcd_Struct_t *lcd);
    Glasslcd_Struct_t lcd;
    sprintf(lcd.string, "Err      %ld", ulLine);
    lcd.command = eGLASSLCD_STRING;
    Glasslcd_WriteStruct(&lcd);
}
/*********************************************************/
void Func_Glasslcd(void)
{
    //---- Scheduler First Init Delay -----
    osSemaphoreId_t InitTime_Sem = osSemaphoreNew(1, 0, NULL);
    osSemaphoreAcquire(InitTime_Sem, 50);
    osSemaphoreDelete(InitTime_Sem);
    //-------------------------------------
    Glasslcd_Struct_t lcd;
    SendConfigMsg_Glasslcd(eGLASSLCD_INIT);
    lcd.command = eGLASSLCD_STRING;
    memset(lcd.string, '8', 13);
    SendDataMsg_Glasslcd(&lcd);
    DBG_PRINTF("GlassLcd lib boot done!");
    //-------------------------------------
#ifdef LIB_CALENDAR
    SendConfigMsg_Calendar(eCALENDAR_ADD_CALLBACK, NULL, Calendar_ISR);
#endif
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
