/*
 *  Buttons Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef __LIB_BUTTONS_H
#define __LIB_BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

struct button_list_t
{
    struct  button_list_t *next;
    void	(*vfPtr)(void);
};
typedef struct button_list_t Button_List_t;

typedef enum
{
    eBUTTONS_DATA_FRAME,
    eBUTTONS_CONFIG_FRAME,
    eBUTTONS_NUMBEROFTYPE
} Buttons_Frame_t;

typedef enum
{
    eBUTTONS_CONFIG_INIT,
    eBUTTONS_CONFIG_DEINIT,
    eBUTTONS_CONFIG_DISABLE,
    eBUTTONS_CONFIG_ENABLE,
    eBUTTONS_CONFIG_CALLBACK,
    eBUTTONS_CONFIG_NUMBEROFTYPES
} Buttons_Config_t;

typedef enum
{
    eRELEASED,
    eNOWPRESSED,
    eSTILLRELEASED,
    eSTILLPRESSED,
    eLONGPRESSED
} Button_State_t;

typedef enum
{
    eBUTTON_1_ID,
    eBUTTON_2_ID,
    eBUTTON_3_ID,
    eBUTTON_4_ID,
    eBUTTON_5_ID,
    eBUTTON_6_ID,
    eBUTTON_7_ID,
    eBUTTON_8_ID,
    eBUTTON_9_ID,
    eBUTTON_ID_NUMBEROFTYPE
} Button_ID_t;

typedef struct
{
    Buttons_Frame_t frame_type;
	  Buttons_Config_t config;
		Button_ID_t button_id;
    void(*vfPtr)(Button_State_t state);
} Buttons_Config_Frame_t;

osStatus_t SendConfigMsg_Buttons(Buttons_Config_t config, Button_ID_t button_id, void* cb_func);

#ifdef __cplusplus
}
#endif
#endif /* __LIB_BUTTONS_H */


