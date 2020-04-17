/*
 *  Modbus Functionality
 *
 *  Created on: Apr 15, 2020
 *
 *  Author: Coskun ERGAN
 */

#include "..\Libraries\lib_modbus\lib_modbus.h"

#ifdef LIB_MODBUS

extern uint8_t ucSlaveID[16];
extern uint8_t ucModelNo;
extern uint16_t *usRegInputBuf[];

uint16_t test = 0;
/*********************************************************/
/*********************************************************/
/*********************************************************/
void Func_Modbus(void)
{
    //-------------------------------------
    strcpy((char *)ucSlaveID, "Coskun ERGAN");
    ucModelNo = 0x10;
    usRegInputBuf[0] = &test;
    //---- Scheduler First Init Delay -----
    osSemaphoreId_t InitTime_Sem = osSemaphoreNew(1, 0, NULL);
    osSemaphoreAcquire(InitTime_Sem, 10);
    osSemaphoreDelete(InitTime_Sem);
    //-------------------------------------
    Modbus_Config_Frame_t msg =
    {
        .config = eMODBUS_INIT,
        .mode = MB_RTU,
        .slave_addres = 10,
        .port = 1,
        .baudrate = 9600,
        .parity = MB_PAR_EVEN
    };
    SendConfigMsg_Modbus(&msg);
    DBG_PRINTF("Modbus lib boot done!");
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
