/*
 *  Buttons Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */


#define LIB_BUTTONS_H

#ifndef __LIB_BUTTONS_H
#define __LIB_BUTTONS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define BUTTONS_MSGQUEUE_OBJECT_SIZE (( sizeof(Buttons_Config_Frame_t) > sizeof(Buttons_Data_Frame_t) ) ? sizeof(Buttons_Config_Frame_t)  : sizeof(Buttons_Data_Frame_t))

typedef enum
{
    eBUTTON_DATA_FRAME,
    eBUTTON_CONFIG_FRAME,
    eBUTTON_NUMBEROFTYPE
} Button_Frame_t;

typedef enum
{
    eBUTTON_INIT,
    eBUTTON_DEINIT,
    eBUTTON_DISABLE,
    eBUTTON_ENABLE,
    eBUTTON_ADD_CALLBACK,
    eBUTTON_DEL_CALLBACK,
    eBUTTON_NUMBEROFTYPES
} Button_Config_t;

typedef enum
{
    eBUTTON_TIMER_START,
    eBUTTON_TIMER_STOP
} Button_Data_t;

typedef enum
{
    eBUTTON_ID_1,
    eBUTTON_ID_2,
    eBUTTON_ID_3,
    eBUTTON_ID_4,
    eBUTTON_ID_5,
    eBUTTON_ID_6,
    eBUTTON_ID_7,
    eBUTTON_ID_8,
    eBUTTON_ID_9,
    eBUTTON_ID_ALL,
    eBUTTON_ID_NUMBEROFTYPE
} Button_ID_t;

typedef enum
{
    eRELEASED,
    ePRESSED,
    eLONGPRESSED
} Button_State_t;

struct button_cb_list_t
{
    struct  button_cb_list_t *next;
    void	(*vfPtr)(Button_State_t state);
};
typedef struct button_cb_list_t Button_Cb_List_t;

typedef struct
{
    Button_Frame_t frame_type;
    Button_Config_t config;
    Button_ID_t button_id;
    void(*vfPtr)(Button_State_t state);
} Buttons_Config_Frame_t;

typedef struct
{
    Button_Frame_t frame_type;
    Button_ID_t button_id;
    Button_Data_t data;
} Buttons_Data_Frame_t;

#define BUTTON_CALLBACK_FUNC(_BUTTON_ID,_STATE) 									 	\
		do{																															\
		extern Button_Cb_List_t Button_CbFunc_List[];										\
		Button_State_t State=_STATE;																		\
    static struct button_cb_list_t *s; 															\
    for(s = SysList_Head((list_t)&Button_CbFunc_List[_BUTTON_ID]);  \
            s != NULL; 																							\
            s = SysList_Item_Next(s)) 															\
    {																																\
        s->vfPtr(State); 																						\
    }																																\
	}while(0); 																												\
 
osStatus_t SendConfigMsg_Buttons(Button_Config_t config, Button_ID_t button_id, void *cb_func);
osStatus_t SendDataMsg_Buttons(Button_Data_t data, Button_ID_t button_id);

#ifdef __cplusplus
}
#endif
#endif /* __LIB_BUTTONS_H */


