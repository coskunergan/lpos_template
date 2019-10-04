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

typedef enum
{
    eBUTTONS_CONFIG_FRAME,
    eBUTTONS_NUMBEROFTYPE
} Buttons_Frame_t;

typedef enum
{
    eBUTTONS_CONFIG_INIT,
    eBUTTONS_CONFIG_DEINIT,
    eBUTTONS_CONFIG_DISABLE,
    eBUTTONS_CONFIG_ENABLE,
    eBUTTONS_CONFIG_ADD_CALLBACK,
    eBUTTONS_CONFIG_DEL_CALLBACK,
    eBUTTONS_CONFIG_NUMBEROFTYPES
} Buttons_Config_t;


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
    eBUTTON_ALL_ID,
    eBUTTON_ID_NUMBEROFTYPE
} Button_ID_t;


typedef enum
{
    eRELEASED,
    ePRESSED
} Button_State_t;

struct button_cb_list_t
{
    struct  button_cb_list_t *next;
    void	(*vfPtr)(Button_State_t state);
};
typedef struct button_cb_list_t Button_Cb_List_t;

typedef struct
{
    Buttons_Frame_t frame_type;
    Buttons_Config_t config;
    Button_ID_t button_id;
    void(*vfPtr)(Button_State_t state);
} Buttons_Config_Frame_t;

#define BUTTON_CALLBACK_FUNC(_BUTTON_ID,_STATE) 									 	\
		do{																															\
		extern Button_Cb_List_t Button_CbFunc_List[];										\
    static struct button_cb_list_t *s; 															\
    for(s = SysList_Head((list_t)&Button_CbFunc_List[_BUTTON_ID]);  \
            s != NULL; 																							\
            s = SysList_Item_Next(s)) 															\
    {																																\
        s->vfPtr(_STATE); 																					\
    }																																\
	}while(0); 																												\
 
osStatus_t SendConfigMsg_Buttons(Buttons_Config_t config, Button_ID_t button_id, void *cb_func);

#ifdef __cplusplus
}
#endif
#endif /* __LIB_BUTTONS_H */


