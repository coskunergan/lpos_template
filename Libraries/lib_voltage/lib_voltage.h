/*
 *  Voltage Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_VOLTAGE_H_
#define LIB_VOLTAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define MSGQUEUE_OBJECT_SIZE (( sizeof(Voltage_Config_Frame_t) > sizeof(Voltage_Data_Frame_t) ) ? sizeof(Voltage_Config_Frame_t)  : sizeof(Voltage_Data_Frame_t))

typedef enum
{
    eVOLTAGE_DATA_FRAME,
    eVOLTAGE_CONFIG_FRAME,
    eVOLTAGE_NUMBEROFTYPE
} Voltage_Frame_t;

typedef enum
{
    eVOLTAGE_INIT,
    eVOLTAGE_DEINIT,
    eVOLTAGE_DISABLE,
    eVOLTAGE_ENABLE,
    eVOLTAGE_NUMBEROFTYPES
} Voltage_Config_t;

typedef enum
{
    eVOLTAGE_REFRESH,
} Voltage_Data_t;

typedef struct
{
    Voltage_Frame_t frame_type;
    Voltage_Config_t config;
    osMutexId_t	hw_mutex;
    uint32_t period_ms;
} Voltage_Config_Frame_t;

typedef struct
{
    Voltage_Frame_t frame_type;
    Voltage_Data_t data;
} Voltage_Data_Frame_t;

osStatus_t SendConfigMsg_Voltage(Voltage_Config_t config, osMutexId_t hw_mutex,  uint32_t period_ms);
osStatus_t SendDataMsg_Voltage(Voltage_Data_t data);
uint32_t Voltage_Read(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_VOLTAGE_H */
