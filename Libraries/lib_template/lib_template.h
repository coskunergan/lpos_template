/*
 *  Template Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_TEMPLATE_H_
#define LIB_TEMPLATE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define TEMPLATE_MSGQUEUE_OBJECT_SIZE (( sizeof(Template_Config_Frame_t) > sizeof(Template_Data_Frame_t) ) ? sizeof(Template_Config_Frame_t)  : sizeof(Template_Data_Frame_t))

typedef enum
{
    eTEMPLATE_DATA_FRAME,
    eTEMPLATE_CONFIG_FRAME,
    eTEMPLATE_NUMBEROFTYPE
} Template_Frame_t;

typedef enum
{
    eTEMPLATE_INIT,
    eTEMPLATE_DEINIT,
    eTEMPLATE_DISABLE,
    eTEMPLATE_ENABLE,
    eTEMPLATE_NUMBEROFTYPES
} Template_Config_t;

typedef enum
{
    eTEMPLATE_ON,
    eTEMPLATE_OFF,
    eTEMPLATE_TOGGLE,
} Template_State_t;

typedef struct
{
    Template_Frame_t frame_type;
    Template_Config_t config;    
} Template_Config_Frame_t;

typedef struct
{
    Template_Frame_t frame_type;
    Template_State_t state;
} Template_Data_Frame_t;

osStatus_t SendDataMsg_Template(Template_State_t state);
osStatus_t SendConfigMsg_Template(Template_Config_t config);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_TEMPLATE_H */
