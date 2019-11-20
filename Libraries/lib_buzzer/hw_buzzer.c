/*
 *  Buzzer Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buzzer.h"
#include "pin_defines.h"

#ifdef LIB_BUZZER

/*********************************************************/
void Buzzer_Hw_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = BUZZER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, BUZZER_PASSIVE);
}
/*********************************************************/
void Buzzer_Hw_DeInit(void)
{
    HAL_GPIO_DeInit(BUZZER_PORT, BUZZER_PIN);
}
/*********************************************************/
void Buzzer_Operation(Buzzer_State_t state)
{
    if(state == eBUZZER_ON)
    {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, BUZZER_ACTIVE);
    }
    else if(state == eBUZZER_OFF)
    {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, BUZZER_PASSIVE);
    }
    else if(state == eBUZZER_TOGGLE)
    {
        HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN);
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
