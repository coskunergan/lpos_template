/*
 *  Temperature Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_TEMPERATURE_H_
#define LIB_TEMPERATURE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define TEMPERATURE_MSGQUEUE_OBJECT_SIZE (( sizeof(Temperature_Config_Frame_t) > sizeof(Temperature_Data_Frame_t) ) ? sizeof(Temperature_Config_Frame_t)  : sizeof(Temperature_Data_Frame_t))

typedef enum
{
    eTEMPERATURE_DATA_FRAME,
    eTEMPERATURE_CONFIG_FRAME,
    eTEMPERATURE_NUMBEROFTYPE
} Temperature_Frame_t;

typedef enum
{
    eTEMPERATURE_INIT,
    eTEMPERATURE_DEINIT,
    eTEMPERATURE_DISABLE,
    eTEMPERATURE_ENABLE,
    eTEMPERATURE_NUMBEROFTYPES
} Temperature_Config_t;

typedef enum
{
    eTEMPERATURE_REFRESH,
} Temperature_Data_t;

typedef struct
{
    Temperature_Frame_t frame_type;
    Temperature_Config_t config;
    osMutexId_t hw_mutex;
    uint32_t period_ms;
} Temperature_Config_Frame_t;

typedef struct
{
    Temperature_Frame_t frame_type;
    Temperature_Data_t data;
} Temperature_Data_Frame_t;

osStatus_t SendConfigMsg_Temperature(Temperature_Config_t config, osMutexId_t hw_mutex, uint32_t period_ms);
osStatus_t SendDataMsg_Temperature(Temperature_Data_t data);
int8_t Temperature_Read(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_TEMPERATURE_H */
