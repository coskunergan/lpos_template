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

typedef struct
{
    Voltage_Frame_t frame_type;
    Voltage_Config_t config;
    uint32_t period_ms;
} Voltage_Config_Frame_t;

osStatus_t SendConfigMsg_Voltage(Voltage_Config_t config, uint32_t period_ms);
uint32_t Voltage_Read(void);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_VOLTAGE_H */
