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
    //-------------------------------------
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
