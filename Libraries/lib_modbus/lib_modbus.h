/*
 *  Modbus Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_MODBUS_H_
#define LIB_MODBUS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define REG_INPUT_START                 ( 1000 )
#define REG_INPUT_NREGS                 ( 64 )

#define REG_HOLDING_START               ( 1 )
#define REG_HOLDING_NREGS               ( 32 )

#define BAUDRATE_ERROR_CORRECT          ( 3 ) // +-% percent

#define BAUD_CORRECTION(x)  ((x) + (x)/100*(BAUDRATE_ERROR_CORRECT))

typedef enum
{
    eMODBUS_DATA_FRAME,
    eMODBUS_CONFIG_FRAME,
    eMODBUS_NUMBEROFTYPE
} Modbus_Frame_t;

typedef enum
{
    eMODBUS_INIT,
    eMODBUS_DEINIT,
    eMODBUS_DISABLE,
    eMODBUS_ENABLE,
    eMODBUS_NUMBEROFTYPES
} Modbus_Config_t;

typedef enum
{
    eMODBUS_TASKWOKEN,
    eMODBUS_TIMER_INIT,
    eMODBUS_TIMER_START,
    eMODBUS_TIMER_STOP,
    eMODBUS_TIMER_CLOSE
} Modbus_Data_t;

typedef struct
{
    Modbus_Frame_t frame_type;
    Modbus_Config_t config;
	  uint32_t baudrate;
} Modbus_Config_Frame_t;

typedef struct
{
    Modbus_Frame_t frame_type;
    Modbus_Data_t data;
    uint32_t timer_period;
} Modbus_Data_Frame_t;

void Modbus_Hw_Init(uint32_t baudrate, uint8_t databit, uint8_t parity);
void Modbus_Hw_Rx_State(uint8_t state);
void Modbus_Hw_Tx_State(uint8_t state);
void Modbus_Hw_Disable(void);
void Modbus_Hw_SendByte(uint8_t byte);
uint8_t Modbus_Hw_GetByte(void);
osStatus_t SendDataMsg_Modbus(Modbus_Data_Frame_t *frame);
osStatus_t SendConfigMsg_Modbus(Modbus_Config_Frame_t *frame);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_MODBUS_H */
