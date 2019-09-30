/*
 *  Library Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "stm32l4xx_hal.h"

typedef struct
{
    uint32_t  key;
    void	(*vfPtr)(void);
} Lib_Start_List_t;

#define LIB_PORT_KEY  0xDEADBEEF

#define FIRST_START_OS(func) \
static void func(void); \
__attribute__((used, section(".lib_obj"))) \
static Lib_Start_List_t lib_start = { LIB_PORT_KEY, &func };

void Lib_Port(void);


