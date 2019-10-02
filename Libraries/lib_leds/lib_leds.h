/*
 *  Leds Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_LEDS_H_
#define LIB_LEDS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
 
 typedef enum
{
    eLEDS_DATA_FRAME,
    eLEDS_CONFIG_FRAME,
    eLEDS_NUMBEROFTYPE
} Led_Frame_t;

typedef enum
{
    eLEDS_CONFIG_INIT,
    eLEDS_CONFIG_DEINIT,
    eLEDS_CONFIG_DISABLE,
    eLEDS_CONFIG_ENABLE,
    eLEDS_CONFIG_NUMBEROFTYPES
} Led_Config_t;

typedef enum
{
    eLED_1_ID,
    eLED_2_ID,
    eLED_3_ID,
    eLED_4_ID,
    eLED_5_ID,
    eLED_6_ID,
    eLED_7_ID,
    eLED_8_ID,
    eLED_9_ID,
    eLED_ID_NUMBEROFTYPE
} Led_ID_t;

typedef enum
{
    eON,
    eOFF,
    eTOGGLE,        
} Led_State_t;

typedef struct
{
    Led_Frame_t frame_type;
	  Led_Config_t config;
} Led_Config_Frame_t;

typedef struct
{
    Led_Frame_t frame_type;
    Led_ID_t led_id;
    Led_State_t state;
} Led_Data_Frame_t;

osStatus_t SendDataMsg_Led(Led_ID_t led_id, Led_State_t state);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_LEDS_H */
