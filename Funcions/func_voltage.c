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
    //---- Scheduler First Init Delay -----
    osSemaphoreId_t InitTime_Sem = osSemaphoreNew(1, 0, NULL);
    osSemaphoreAcquire(InitTime_Sem, 60);
    osSemaphoreDelete(InitTime_Sem);
    //-------------------------------------
    SendConfigMsg_Voltage(eVOLTAGE_INIT, Get_Hw_Mutex(MUTEX_ADC), 10000);
    DBG_PRINTF("Voltage lib boot done!");
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
