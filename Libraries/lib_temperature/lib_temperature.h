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

typedef struct
{
    Temperature_Frame_t frame_type;
    Temperature_Config_t config;
    uint32_t period_ms;
} Temperature_Config_Frame_t;

osStatus_t SendConfigMsg_Temperature(Temperature_Config_t config, uint32_t period_ms);
int8_t Temperature_Read(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_TEMPERATURE_H */
