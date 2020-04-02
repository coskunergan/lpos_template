/*
 *  Library Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef SYS_LIB_H_
#define SYS_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

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

void SysLib_Init(void);

#ifdef __cplusplus
}
#endif

#endif /* SYS_LIB_H_ */
