/*
 *  Library Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "sys_lib.h"


FIRST_START_OS(SysLib_Start);
/*********************************************************/
/*********************************************************/
/*********************************************************/
static void SysLib_Start(void)
{
    __NOP();
}
/*********************************************************/
void SysLib_Init(void)
{
    Lib_Start_List_t *start_ptr = &lib_start;

    while(start_ptr->key == LIB_PORT_KEY)
    {
        start_ptr->vfPtr();
        start_ptr++;
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

