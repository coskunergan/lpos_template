/*
 *  Boot Librarys
 *
 *  Created on: Oct 12, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef BOOT_H_
#define BOOT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef struct
{
    uint32_t ReferanceVoltageLevelmV;
    int16_t TemperatureLevelCelsius;
    uint8_t CPULevelPercent;
} GlobalStats_t;

typedef enum
{
    MUTEX_ADC,
    MUTEX_UART
} Hw_Mutex_t;

osMutexId_t Get_Hw_Mutex(Hw_Mutex_t mt);

#define LIB_LEDS
#define LIB_BUZZER
#define LIB_BUTTONS
#define LIB_GLASSLCD
#define LIB_CALENDAR
#define LIB_VOLTAGE
#define LIB_TEMPERATURE
#define LIB_MODBUS

#ifdef __cplusplus
}
#endif

#endif /* __BOOT_H */

