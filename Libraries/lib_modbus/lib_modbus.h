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

#define BAUDRATE_ERROR_CORRECT          ( 3 ) // +-% percent (3 => ASCII)

#define BAUD_CORRECTION(x)  ((x) + (x)/100*(BAUDRATE_ERROR_CORRECT))

#define MODBUS_MSGQUEUE_OBJECT_SIZE (( sizeof(Modbus_Config_Frame_t) > sizeof(Modbus_Data_Frame_t) ) ? sizeof(Modbus_Config_Frame_t)  : sizeof(Modbus_Data_Frame_t))

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
    eMODBUS_TIMER_CLOSE,
    eMODBUS_ADD_RO_REG,
    eMODBUS_ADD_RW_REG,
    eMODBUS_DEL_REG
} Modbus_Data_t;

typedef enum
{
    MB_RTU,                     /*!< RTU transmission mode. */
    MB_ASCII,                   /*!< ASCII transmission mode. */
    MB_TCP                      /*!< TCP mode. */
} eMBMode;

typedef enum
{
    MB_PAR_NONE,                /*!< No parity. */
    MB_PAR_ODD,                 /*!< Odd parity. */
    MB_PAR_EVEN                 /*!< Even parity. */
} eMBParity;

typedef enum
{
    MB_ENOERR,                  /*!< no error. */
    MB_ENOREG,                  /*!< illegal register address. */
    MB_EINVAL,                  /*!< illegal argument. */
    MB_EPORTERR,                /*!< porting layer error. */
    MB_ENORES,                  /*!< insufficient resources. */
    MB_EIO,                     /*!< I/O error. */
    MB_EILLSTATE,               /*!< protocol stack in illegal state. */
    MB_ETIMEDOUT                /*!< timeout error occurred. */
} eMBErrorCode;

typedef struct
{
    Modbus_Frame_t frame_type;
    Modbus_Config_t config;
    eMBMode mode;
    uint8_t port;
    uint8_t slave_addres;
    uint32_t baudrate;
    eMBParity parity;
} Modbus_Config_Frame_t;

typedef struct
{
    Modbus_Frame_t frame_type;
    Modbus_Data_t data;
    uint32_t timer_period;
    void *ptr;
    uint16_t length;
    uint16_t addres;
} Modbus_Data_Frame_t;

void Modbus_Hw_Init(uint8_t port, uint32_t baudrate, uint8_t databit, uint8_t parity);
void Modbus_Hw_Rx_State(uint8_t state);
void Modbus_Hw_Tx_State(uint8_t state);
void Modbus_Hw_Disable(void);
void Modbus_Hw_SendByte(uint8_t byte);
uint8_t Modbus_Hw_GetByte(void);
osStatus_t SendDataMsg_Modbus(Modbus_Data_Frame_t *frame);
osStatus_t SendConfigMsg_Modbus(Modbus_Config_Frame_t *frame);
eMBErrorCode eMB_AddReadOnlyReg(void *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs);
eMBErrorCode eMB_AddReadWriteReg(void *pucRegBuffer, uint16_t usAddress, uint16_t usNRegs);
eMBErrorCode eMB_DeleteRegister(uint16_t usAddress, uint16_t usNRegs);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_MODBUS_H */
