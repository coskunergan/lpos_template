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
	
#define LED_MSGQUEUE_OBJECT_SIZE (( sizeof(Led_Config_Frame_t) > sizeof(Led_Data_Frame_t) ) ? sizeof(Led_Config_Frame_t)  : sizeof(Led_Data_Frame_t))

typedef enum
{
    eLED_DATA_FRAME,
    eLED_CONFIG_FRAME,
    eLED_NUMBEROFTYPE
} Led_Frame_t;

typedef enum
{
    eLED_INIT,
    eLED_DEINIT,
    eLED_DISABLE,
    eLED_ENABLE,
    eLED_NUMBEROFTYPES
} Led_Config_t;

typedef enum
{
    eLED_ID_1,
    eLED_ID_2,
    eLED_ID_3,
    eLED_ID_4,
    eLED_ID_5,
    eLED_ID_6,
    eLED_ID_7,
    eLED_ID_8,
    eLED_ID_9,
    eLED_ID_ALL,
    eLED_ID_NUMBEROFTYPE
} Led_ID_t;

typedef enum
{
    eLED_ON,
    eLED_OFF,
    eLED_TOGGLE,
} Led_State_t;

typedef struct
{
    Led_Frame_t frame_type;
    Led_Config_t config;
    Led_ID_t  led_id;
} Led_Config_Frame_t;

typedef struct
{
    Led_Frame_t frame_type;
    Led_ID_t led_id;
    Led_State_t state;
} Led_Data_Frame_t;

osStatus_t SendDataMsg_Led(Led_ID_t led_id, Led_State_t state);
osStatus_t SendConfigMsg_Led(Led_Config_t config, Led_ID_t led_id);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_LEDS_H */
