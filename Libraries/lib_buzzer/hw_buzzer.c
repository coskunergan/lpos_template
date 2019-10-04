/*
 *  Buzzer Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buzzer.h"
#include "pin_defines.h"

/*********************************************************/
void Buzzer_Hw_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = BUZZER_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(BUZZER_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
}
/*********************************************************/
void Buzzer_Hw_DeInit(void)
{
    HAL_GPIO_DeInit(BUZZER_PORT, BUZZER_PIN);
}
/*********************************************************/
void Buzzer_Operation(Buzzer_State_t state)
{
    if(state == eON)
    {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_RESET);
    }
    else if(state == eOFF)
    {
        HAL_GPIO_WritePin(BUZZER_PORT, BUZZER_PIN, GPIO_PIN_SET);
    }
    else if(state == eTOGGLE)
    {
        HAL_GPIO_TogglePin(BUZZER_PORT, BUZZER_PIN);
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
