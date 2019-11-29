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
#define MSGQUEUE_OBJECT_SIZE sizeof(Modbus_Data_Frame_t)

static osMessageQueueId_t mq_id;

static osTimerId_t MB_Timer_ID;
static uint32_t MB_Timer_Period;
uint8_t SleepAbortWhileTransmitAndReceive = FALSE;

uint16_t   usRegInputStart = REG_INPUT_START;
uint16_t   usRegInputBuf[REG_INPUT_NREGS];
uint16_t   usRegHoldingStart = REG_HOLDING_START;
uint16_t   usRegHoldingBuf[REG_HOLDING_NREGS];

extern void Modbus_Hw_DeInit(void);
static void StartTask(void *argument);

osThreadId_t modbus_task;

/*********************************************************/
/*********************************************************/
/*********************************************************/
static void Lib_Init(void)
{
    mq_id = osMessageQueueNew(MSGQUEUE_OBJECTS, MSGQUEUE_OBJECT_SIZE, NULL);

    const osThreadAttr_t defaultTask_attributes =
    {
        .name = "ModbusTask",
        .priority = (osPriority_t) osPriorityNormal,
        .stack_size = 388
    };
    if(osThreadNew(StartTask, NULL, &defaultTask_attributes) == NULL)
    {
        Error_Handler();
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
    int             iRegIndex;

    if((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegInputStart);
        while(usNRegs > 0)
        {
            *pucRegBuffer++ = (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ = (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
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
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if((usAddress >= REG_HOLDING_START) && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegHoldingStart);
        switch(eMode)
        {
            case MB_REG_READ:
                while(usNRegs > 0)
                {
                    *pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] >> 8);
                    *pucRegBuffer++ = (unsigned char)(usRegHoldingBuf[iRegIndex] & 0xFF);
                    iRegIndex++;
                    usNRegs--;
                }
                break;

            case MB_REG_WRITE:
                while(usNRegs > 0)
                {
                    usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                    usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                    iRegIndex++;
                    usNRegs--;
                }
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
void MB_Timer_Callback(void *arg)
{
    uint8_t bTaskWoken = FALSE;
    Modbus_Data_Frame_t msg;

    bTaskWoken = pxMBPortCBTimerExpired();

    SleepAbortWhileTransmitAndReceive = FALSE;

    if(bTaskWoken == TRUE)
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
    uint8_t msg[MSGQUEUE_OBJECT_SIZE];

    const uint8_t ucSlaveID[] = { 0xAA, 0xBB, 0xCC };

    for(;;)
    {
        if(osMessageQueueGet(mq_id, msg, NULL, (SleepAbortWhileTransmitAndReceive || vMBPortEventCount()) ? 0 : osWaitForever) == osOK)
        {
            if(*msg == eMODBUS_CONFIG_FRAME)
            {
                config_msg = (Modbus_Config_Frame_t *)msg;
                switch(config_msg->config)
                {
                    case eMODBUS_INIT:
                        if(MB_ENOERR != eMBInit(MB_ASCII, 0x0A, 1, BAUD_CORRECTION(config_msg->baudrate), MB_PAR_EVEN))
                        {
                            /* Can not initialize. Add error handling code here. */
                            Error_Handler();
                        }
                        else
                        {
                            if(MB_ENOERR != eMBSetSlaveID(0x34, TRUE, ucSlaveID, 3))
                            {
                                /* Can not set slave id. Check arguments */
                                Error_Handler();
                            }
                            else if(MB_ENOERR != eMBEnable())
                            {
                                /* Enable failed. */
                                Error_Handler();
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
                        MB_Timer_Period = data_msg->timer_period;
                        MB_Timer_ID = osTimerNew(MB_Timer_Callback, osTimerOnce, NULL, NULL);
                        if(MB_Timer_ID  == NULL)
                        {
                            Error_Handler();
                        }
                        break;
                    case eMODBUS_TIMER_START:
                        if(osOK != osTimerStart(MB_Timer_ID, MB_Timer_Period))
                        {
                            Error_Handler();
                        }
                        SleepAbortWhileTransmitAndReceive = TRUE;
                        break;
                    case eMODBUS_TIMER_STOP:
                        if(osTimerIsRunning(MB_Timer_ID) && (osOK != osTimerStop(MB_Timer_ID)))
                        {
                            Error_Handler();
                        }
                        SleepAbortWhileTransmitAndReceive = FALSE;
                        break;
                    case eMODBUS_TIMER_CLOSE:
                        if(osTimerIsRunning(MB_Timer_ID) && (osOK != osTimerDelete(MB_Timer_ID)))
                        {
                            Error_Handler();
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
            usRegInputBuf[0]++;
						osThreadYield();
        }
        //------------------------------------------------------------
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
