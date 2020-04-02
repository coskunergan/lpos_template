/*
 * FreeModbus Libary: Coskun ERGAN LPOS STM32 Application
 * Copyright (C) 2010 Christian Walter <cwalter@embedded-solutions.at>
 *
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id$
 */

/* ----------------------- System includes ----------------------------------*/

/* ----------------------- Modbus includes ----------------------------------*/
#include "..\..\Libraries\lib_modbus\port\port.h"
#include "..\..\Libraries\lib_modbus\modbus\include\mb.h"
#include "..\..\Libraries\lib_modbus\modbus\include\mbport.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/
static uint32_t Timer_Period;
static uint8_t Timer_Sparse;
static uint8_t Timer_Sparse_Count;
/* ----------------------- Start implementation -----------------------------*/

uint8_t xMBPortTimersInit(uint16_t usTim1Timerout50us, uint32_t ulBaudRate)
{
    Modbus_Data_Frame_t msg;

    Timer_Period = (usTim1Timerout50us / 20) + 1; // minimum timeout 1ms

    Timer_Sparse_Count = 0;
    if(ulBaudRate > 30000)
    {
        Timer_Sparse = 4;
    }
    else if(ulBaudRate > 6000)
    {
        Timer_Sparse = 2;
    }
    else
    {
        Timer_Sparse = 1;
    }

    msg.data = eMODBUS_TIMER_INIT;
    msg.timer_period = Timer_Period;
    SendDataMsg_Modbus(&msg);

    return MB_TRUE;
}

void vMBPortTimersEnable()
{
    if((Timer_Sparse_Count++ % Timer_Sparse) == 0)
    {
        Modbus_Data_Frame_t msg;
        msg.data = eMODBUS_TIMER_START;
        SendDataMsg_Modbus(&msg);
    }
}

void vMBPortTimersDisable()
{
    Timer_Sparse_Count = 0;
    Modbus_Data_Frame_t msg;
    msg.data = eMODBUS_TIMER_STOP;
    SendDataMsg_Modbus(&msg);
}

void vMBPortTimerClose(void)
{
    Modbus_Data_Frame_t msg;
    msg.data = eMODBUS_TIMER_CLOSE;
    SendDataMsg_Modbus(&msg);
}

void vMBPortTimersDelay(uint16_t usTimeOutMS)
{
		osSemaphoreId_t MB_Sem = osSemaphoreNew(1,0,NULL);
		osSemaphoreAcquire(MB_Sem,usTimeOutMS);	
		osSemaphoreDelete(MB_Sem);		
    //osDelay(usTimeOutMS);			
}

