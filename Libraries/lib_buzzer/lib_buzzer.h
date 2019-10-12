/*
 *  Buzzer Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_BUZZER_H_
#define LIB_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#define BUZZER_STRUCT_VERSION	1

#define BUZZER_TONE_A 0
#define BUZZER_TONE_NONE 255
#define BUZZER_SHORT_BEEP 80
#define BUZZER_LONG_BEEP 500

typedef enum
{
    eBUZZER_DATA_FRAME,
    eBUZZER_CONFIG_FRAME,
    eBUZZER_NUMBEROFTYPE
} Buzzer_Frame_t;

typedef enum
{
    eBUZZER_INIT,
    eBUZZER_DEINIT,
    eBUZZER_DISABLE,
    eBUZZER_ENABLE,
    eBUZZER_NUMBEROFTYPES
} Buzzer_Config_t;

typedef enum
{
    eBUZZER_ON,
    eBUZZER_OFF,
    eBUZZER_TOGGLE,
} Buzzer_State_t;

typedef struct
{
    Buzzer_Frame_t frame_type;
    Buzzer_Config_t config;
} Buzzer_Config_Frame_t;

typedef struct
{
    Buzzer_Frame_t frame_type;
    uint8_t tone;
    uint16_t active_time;
} Buzzer_Data_Frame_t;

osStatus_t SendDataMsg_Buzzer(uint8_t tone, uint16_t active_time);
osStatus_t SendConfigMsg_Buzzer(Buzzer_Config_t config);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_BUZZER_H */


