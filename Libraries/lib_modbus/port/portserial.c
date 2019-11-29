/*
 * FreeModbus Libary: Coskun ERGAN STM32 LPOS Application
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
//#include <stdlib.h>
//#include <FreeRTOS.h>
//#include <task.h>
//#include <queue.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "..\..\lib_modbus\modbus\include\mb.h"
#include "..\..\Libraries\lib_modbus\port\port.h"
#include "..\..\Libraries\lib_modbus\modbus\include\mbport.h"

/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Static variables ---------------------------------*/

uint8_t xMBPortSerialInit(uint8_t ucPORT, uint32_t ulBaudRate, uint8_t ucDataBits, eMBParity eParity)
{
    Modbus_Hw_Init(ulBaudRate, ucDataBits, eParity);
    return TRUE;
}

void vMBPortSerialEnable(uint8_t xRxEnable, uint8_t xTxEnable)
{
    Modbus_Hw_Rx_State(xRxEnable);
    Modbus_Hw_Tx_State(xTxEnable);
}

void vMBPortSerialClose(void)
{
    Modbus_Hw_Disable();
}

uint8_t xMBPortSerialPutByte(uint8_t ucByte)
{
    Modbus_Hw_SendByte(ucByte);
    return TRUE;
}

uint8_t xMBPortSerialGetByte(uint8_t *pucByte)
{
    *pucByte = Modbus_Hw_GetByte();
    return TRUE;
}

