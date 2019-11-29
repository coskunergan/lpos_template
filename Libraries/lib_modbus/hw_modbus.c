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

#define UART_PREEMPTPRIORITY 	15
#define UART_SUBPRIORITY  	0

#define WAKEUP_FROM_ADDRESS

extern uint8_t SleepAbortWhileTransmitAndReceive;

static uint8_t rx_byte;
static uint8_t tx_byte;

uint32_t MB_Uart_Fail_Counter = 0;

static struct isr_list_t uart_isr_handle;

static UART_HandleTypeDef UartHandle;
/*********************************************************/
static void Uart_ISR(void)// Uart ISR Handler
{
    HAL_UART_IRQHandler(&UartHandle);
}
/*********************************************************/
void HAL_UART_ErrorCallback(UART_HandleTypeDef *UartHandle)	// fail interrupt
{
    MB_Uart_Fail_Counter++;
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_SET);// TEST FAIL
//	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, GPIO_PIN_RESET);// TEST FAIL
//	Error_Handler();
}
/*********************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) 	// trasmit done interrupt
{
    Modbus_Data_Frame_t msg;
    uint8_t bTaskWoken = FALSE;

    if(UartHandle->Instance == MB_UART)
    {
        bTaskWoken = pxMBFrameCBTransmitterEmpty();

        if(bTaskWoken == TRUE)
        {
            msg.data = eMODBUS_TASKWOKEN;

            SendDataMsg_Modbus(&msg);
        }
    }
}
/*********************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) // receive done interrupt
{
    Modbus_Data_Frame_t msg;
    uint8_t bTaskWoken = FALSE;

    if(UartHandle->Instance == MB_UART)
    {
        bTaskWoken = pxMBFrameCBByteReceived();

        HAL_UART_Receive_IT(UartHandle, &rx_byte, 1);

        if(bTaskWoken == TRUE)
        {
            msg.data = eMODBUS_TASKWOKEN;
            SendDataMsg_Modbus(&msg);
        }
    }
}
/*********************************************************/
void Modbus_Hw_SendByte(uint8_t byte)
{
    tx_byte = byte;
    HAL_UART_Transmit_IT(&UartHandle, &tx_byte, 1);
}
/*********************************************************/
uint8_t Modbus_Hw_GetByte(void)
{
    return rx_byte;
}
/*********************************************************/
void Modbus_Hw_Init(uint32_t baudrate, uint8_t databit, uint8_t parity)
{
    GPIO_InitTypeDef  GPIO_InitStruct = {0};

    SysIsr_Add(&uart_isr_handle, Uart_ISR, MB_UART_ISR_HANDLER);

    UartHandle.Instance = MB_UART;
    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-1- Enable peripherals and GPIO Clocks #################################*/
    /* Enable GPIO TX/RX clock */
    MB_UART_TX_CLK_ENABLE();
    MB_UART_RX_CLK_ENABLE();
    MB_UART_EN_CLK_ENABLE();

    MB_UART_RCC_CONFIG(MB_UART_CLOCK_SOURCE);

    /* Enable USARTx clock */
    MB_UART_CLK_ENABLE();

    /*##-2- Configure peripheral GPIO ##########################################*/

    GPIO_InitStruct.Pin       = MB_UART_TX_PIN;
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = MB_UART_TX_AF;

    HAL_GPIO_Init(MB_UART_TX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin       = MB_UART_RX_PIN;
    GPIO_InitStruct.Alternate = MB_UART_RX_AF;
    HAL_GPIO_Init(MB_UART_RX_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = MB_UART_EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MB_UART_EN_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(MB_UART_EN_PORT, MB_UART_EN_PIN, GPIO_PIN_RESET);

    /*##-3- Configure UART ####################################################*/

    UartHandle.Init.BaudRate = baudrate;

    switch(databit)
    {
        case 7:
        //UartHandle.Init.WordLength = UART_WORDLENGTH_7B;
        //break;
        case 8:
            UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
            break;
        default:
            Error_Handler();
            break;
    }

    switch(parity)
    {
        case MB_PAR_NONE:
            UartHandle.Init.Parity = UART_PARITY_NONE;
            UartHandle.Init.StopBits = UART_STOPBITS_2;
            break;
        case MB_PAR_ODD:
            UartHandle.Init.Parity = UART_PARITY_ODD;
            UartHandle.Init.StopBits = UART_STOPBITS_1;
            break;
        case MB_PAR_EVEN:
            UartHandle.Init.Parity = UART_PARITY_EVEN;
            UartHandle.Init.StopBits = UART_STOPBITS_1;
            break;
        default:
            Error_Handler();
            break;
    }
    UartHandle.Init.Mode = UART_MODE_TX_RX;
    UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    UartHandle.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if(HAL_UART_Init(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-4- Configure Interrput ####################################################*/

    /* NVIC for USART */
    HAL_NVIC_SetPriority(MB_UART_IRQn, UART_SUBPRIORITY, UART_PREEMPTPRIORITY);
    HAL_NVIC_EnableIRQ(MB_UART_IRQn);

    HAL_UARTEx_EnableStopMode(&UartHandle);

    HAL_UART_Receive_IT(&UartHandle, &rx_byte, 1);
}
/*********************************************************/
void Modbus_Hw_DeInit()
{
    HAL_UART_Abort_IT(&UartHandle);

    SysIsr_Remove(&uart_isr_handle, MB_UART_ISR_HANDLER);

    MB_UART_CLK_DISABLE();

    HAL_GPIO_DeInit(MB_UART_TX_PORT, MB_UART_TX_PIN);
    HAL_GPIO_DeInit(MB_UART_RX_PORT, MB_UART_RX_PIN);
		HAL_GPIO_DeInit(MB_UART_EN_PORT, MB_UART_EN_PIN);

    HAL_NVIC_DisableIRQ(MB_UART_IRQn);

    if(HAL_UART_DeInit(&UartHandle) != HAL_OK)
    {
        Error_Handler();
    }
}
/*********************************************************/
void Modbus_Hw_Rx_State(uint8_t state)
{
    if(state == TRUE)
    {
        HAL_GPIO_WritePin(MB_UART_EN_PORT, MB_UART_EN_PIN, GPIO_PIN_RESET);// RX mode on
        HAL_UART_Receive_IT(&UartHandle, &rx_byte, 1);
    }
    else
    {
        HAL_UART_AbortReceive_IT(&UartHandle);
    }
}
/*********************************************************/
void Modbus_Hw_Tx_State(uint8_t state)
{
    if(state == TRUE)
    {
        HAL_GPIO_WritePin(MB_UART_EN_PORT, MB_UART_EN_PIN, GPIO_PIN_SET);// TX mode on
        SleepAbortWhileTransmitAndReceive = TRUE;
        pxMBFrameCBTransmitterEmpty();
    }
    else
    {
        SleepAbortWhileTransmitAndReceive = FALSE;
        HAL_UART_AbortTransmit_IT(&UartHandle);
    }
}
/*********************************************************/
void Modbus_Hw_Disable(void)
{
    HAL_UART_Abort_IT(&UartHandle);
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
