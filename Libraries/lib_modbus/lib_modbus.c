/*
 *  Modbus Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_modbus.h"
#include "pin_defines.h"

#ifdef LIB_MODBUS

#include "..\..\Libraries\lib_modbus\modbus\include\mb.h"
#include "..\..\Libraries\lib_modbus\modbus\include\mbport.h"
#include "..\..\Libraries\lib_modbus\modbus\rtu\mbrtu.h"

FIRST_START_OS(Lib_Init);

#define MSGQUEUE_OBJECTS  8

static osMessageQueueId_t mq_id;

uint8_t ucSlaveID[16] = {0};
uint8_t ucModelNo = 0x10;

static osTimerId_t MB_Timer_ID;
static uint32_t MB_Timer_Period;
uint8_t SleepAbortWhileTransmit = MB_FALSE;

uint16_t   usRegInputStart = REG_INPUT_START;
uint16_t   *usRegInputBuf[REG_INPUT_NREGS];
uint16_t   usRegHoldingStart = REG_HOLDING_START;
uint16_t   *usRegHoldingBuf[REG_HOLDING_NREGS];

extern void Modbus_Hw_DeInit(void);
extern void Func_Modbus(void);
static void StartTask(void *argument);

osThreadId_t modbus_task;

extern uint16_t test;
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MODBUS_MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "ModbusTask",
        .priority = (osPriority_t) osPriorityRealtime,
        .stack_size = 512
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        vAssertCalled(__FILE__, __LINE__);
    }
}
/*********************************************************/
osStatus_t SendDataMsg_Modbus(Modbus_Data_Frame_t *frame)
{
    Modbus_Data_Frame_t msg = *frame;

    msg.frame_type = eMODBUS_DATA_FRAME;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
osStatus_t SendConfigMsg_Modbus(Modbus_Config_Frame_t *frame)
{
    Modbus_Config_Frame_t msg = *frame;

    msg.frame_type = eMODBUS_CONFIG_FRAME;

    if(osMessageQueueGetSpace(mq_id) != 0)
    {
        return osMessageQueuePut(mq_id, &msg, osPriorityNone, 0);
    }
    return osErrorNoMemory;
}
/*********************************************************/
eMBErrorCode eMBRegInputCB(uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint16_t iRegIndex;

    if((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (uint16_t)(usAddress - usRegInputStart);
        while(usNRegs > 0)
        {
            if(usRegInputBuf[iRegIndex] != NULL)
            {
                *pucRegBuffer++ = (uint8_t)(*usRegInputBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (uint8_t)(*usRegInputBuf[iRegIndex] & 0xFF);
            }
            else
            {
                *pucRegBuffer++ = 0;
                *pucRegBuffer++ = 0;
                //eStatus = MB_ENOREG;
            }
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*********************************************************/
eMBErrorCode eMBRegHoldingCB(uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode)
{
    eMBErrorCode eStatus = MB_ENOERR;
    uint16_t iRegIndex;

    if((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = (uint16_t)(usAddress - usRegHoldingStart);
        switch(eMode)
        {
            case MB_REG_READ:
                osKernelLock();
                while(usNRegs > 0)
                {
                    if(usRegHoldingBuf[iRegIndex] != NULL)
                    {
                        *pucRegBuffer++ = (uint8_t)(*usRegHoldingBuf[iRegIndex] >> 8);
                        *pucRegBuffer++ = (uint8_t)(*usRegHoldingBuf[iRegIndex] & 0xFF);
                    }
                    else
                    {
                        *pucRegBuffer++ = 0;
                        *pucRegBuffer++ = 0;
                        //eStatus = MB_ENOREG;
                    }
                    iRegIndex++;
                    usNRegs--;
                }
                osKernelUnlock();
                break;

            case MB_REG_WRITE:
                osKernelLock();
                while(usNRegs > 0)
                {
                    if(usRegHoldingBuf[iRegIndex] != NULL)
                    {
                        *usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                        *usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    }
                    else
                    {
                        //eStatus = MB_ENOREG;
                    }
                    iRegIndex++;
                    usNRegs--;
                }
                osKernelUnlock();
                break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
/*********************************************************/
eMBErrorCode eMBRegCoilsCB(uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNCoils, eMBRegisterMode eMode)
{
    return MB_ENOREG;
}
/*********************************************************/
eMBErrorCode eMBRegDiscreteCB(uint8_t *pucRegBuffer, uint16_t usAddress, uint16_t usNDiscrete)
{
    return MB_ENOREG;
}
/*********************************************************/
eMBErrorCode eMB_AddReadOnlyReg(void *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs)
{
    eMBErrorCode  eStatus = MB_ENOERR;
    uint16_t  iRegIndex;
    uint16_t *RegPtr = pucRegBuffer;

    if(pucRegBuffer == NULL)
    {
        eStatus = MB_EINVAL;
    }
    else if((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (uint16_t)(usAddress - usRegInputStart);
        osKernelLock();
        while(usNRegs > 0)
        {
            usRegInputBuf[iRegIndex] =	RegPtr++;
            iRegIndex++;
            usNRegs--;
        }
        osKernelUnlock();
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*********************************************************/
eMBErrorCode eMB_AddReadWriteReg(void *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs)
{
    eMBErrorCode  eStatus = MB_ENOERR;
    uint16_t  iRegIndex;
    uint16_t *RegPtr = pucRegBuffer;

    if(pucRegBuffer == NULL)
    {
        eStatus = MB_EINVAL;
    }
    else if((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = (uint16_t)(usAddress - usRegHoldingStart);
        osKernelLock();
        while(usNRegs > 0)
        {
            usRegHoldingBuf[iRegIndex] =	RegPtr++;
            iRegIndex++;
            usNRegs--;
        }
        osKernelUnlock();
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*********************************************************/
eMBErrorCode eMB_DeleteRegister(uint16_t usAddress, uint16_t usNRegs)
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint16_t iRegIndex;

    if((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (uint16_t)(usAddress - usRegInputStart);
        osKernelLock();
        while(usNRegs > 0)
        {
            usRegInputBuf[iRegIndex] =	0;
            iRegIndex++;
            usNRegs--;
        }
        osKernelUnlock();
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}
/*********************************************************/
void MB_Timer_Callback(void *arg)
{
    uint8_t bTaskWoken = MB_FALSE;
    Modbus_Data_Frame_t msg;

    bTaskWoken = pxMBPortCBTimerExpired();

    if(bTaskWoken == MB_TRUE)
    {
        msg.data = eMODBUS_TASKWOKEN;
        SendDataMsg_Modbus(&msg);
    }
}
/*********************************************************/
static void StartTask(void *argument)
{
    Modbus_Data_Frame_t *data_msg;
    Modbus_Config_Frame_t *config_msg;
    uint8_t msg[MODBUS_MSGQUEUE_OBJECT_SIZE];

    Func_Modbus();

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, (SleepAbortWhileTransmit || vMBPortEventCount()) ? 1 : osWaitForever) == osOK)
        {
            if(*msg == eMODBUS_CONFIG_FRAME)
            {
                config_msg = (Modbus_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eMODBUS_INIT:
                        if(MB_ENOERR != eMBInit(config_msg->mode, config_msg->slave_addres, config_msg->port, BAUD_CORRECTION(config_msg->baudrate), config_msg->parity))
                        {
                            /* Can not initialize. Add error handling code here. */
                            vAssertCalled(__FILE__, __LINE__);
                        }
                        else
                        {
                            if(MB_ENOERR != eMBSetSlaveID(ucModelNo, MB_TRUE, ucSlaveID, sizeof(ucSlaveID)))
                            {
                                /* Can not set slave id. Check arguments */
                                vAssertCalled(__FILE__, __LINE__);
                            }
                            else if(MB_ENOERR != eMBEnable())
                            {
                                /* Enable failed. */
                                vAssertCalled(__FILE__, __LINE__);
                            }
                        }
                        break;
                    case eMODBUS_DEINIT:
                        Modbus_Hw_DeInit();
                        break;
                    case eMODBUS_ENABLE:
                        eMBEnable();
                        break;
                    case eMODBUS_DISABLE:
                        eMBDisable();
                        break;
                    default:
                        break;
                }
            }
            else if(*msg == eMODBUS_DATA_FRAME)
            {
                data_msg = (Modbus_Data_Frame_t *)msg;
                switch(data_msg->data)
                {
                    case eMODBUS_TASKWOKEN:
                        break;
                    case eMODBUS_TIMER_INIT:
                        if(MB_Timer_ID  == NULL)
                        {
                            MB_Timer_Period = data_msg->timer_period;
                            MB_Timer_ID = osTimerNew(MB_Timer_Callback, osTimerOnce, NULL, NULL);
                            if(MB_Timer_ID  == NULL)
                            {
                                vAssertCalled(__FILE__, __LINE__);
                            }
                        }
                        break;
                    case eMODBUS_TIMER_START:
                        if(osOK != osTimerStart(MB_Timer_ID, MB_Timer_Period))
                        {
                            vAssertCalled(__FILE__, __LINE__);
                        }
                        break;
                    case eMODBUS_TIMER_STOP:
                        if(osTimerIsRunning(MB_Timer_ID) && (osOK != osTimerStop(MB_Timer_ID)))
                        {
                            vAssertCalled(__FILE__, __LINE__);  // burada bazen patliyor.
                        }
                        break;
                    case eMODBUS_TIMER_CLOSE:
                        if(osTimerIsRunning(MB_Timer_ID) && (osOK != osTimerDelete(MB_Timer_ID)))
                        {
                            vAssertCalled(__FILE__, __LINE__);
                        }
                        break;
                    case eMODBUS_ADD_RO_REG:
                        if(MB_ENOERR != eMB_AddReadOnlyReg(data_msg->ptr, data_msg->addres, data_msg->length))
                        {
                            vAssertCalled(__FILE__, __LINE__);
                        }
                        break;
                    case eMODBUS_ADD_RW_REG:
                        if(MB_ENOERR != eMB_AddReadWriteReg(data_msg->ptr, data_msg->addres, data_msg->length))
                        {
                            vAssertCalled(__FILE__, __LINE__);
                        }
                        break;
                    case eMODBUS_DEL_REG:
                        if(MB_ENOERR != eMB_DeleteRegister(data_msg->addres, data_msg->length))
                        {
                            vAssertCalled(__FILE__, __LINE__);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
        //-----------------------------------------------------------
        if(osMessageQueueGetCount(mq_id) == 0)
        {
            eMBPoll();
            test++;
            osThreadYield();
        }
        //------------------------------------------------------------
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
