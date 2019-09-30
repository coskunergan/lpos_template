/*
 *  Library Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_port.h"

FIRST_START_OS(Pre_Lib_Port);

static void Pre_Lib_Port(void)
{
    __NOP();
}

void Lib_Port(void)
{
    Lib_Start_List_t *start_ptr = &lib_start;

    while(start_ptr->key == LIB_PORT_KEY)
    {
        start_ptr->vfPtr();
        start_ptr++;
    }
}

