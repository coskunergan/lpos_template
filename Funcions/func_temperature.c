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
void Temperature_Refresh_Event(void)
{

}
/*********************************************************/
void Func_Temperature(void)
{
    //---- Scheduler First Init Delay -----
    osSemaphoreId_t InitTime_Sem = osSemaphoreNew(1, 0, NULL);
    osSemaphoreAcquire(InitTime_Sem, 70);
    osSemaphoreDelete(InitTime_Sem);
    //-------------------------------------
    SendConfigMsg_Temperature(eTEMPERATURE_INIT, Get_Hw_Mutex(MUTEX_ADC), 10000);
    DBG_PRINTF("Temperature lib boot done!");
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

