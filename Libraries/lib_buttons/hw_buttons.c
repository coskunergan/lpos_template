/*
 *  Buttons Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_buttons.h"
#include "pin_defines.h"

#define BUTTON_PREEMPTPRIORITY 	15
#define BUTTON_SUBPRIORITY  	0

static struct isr_list_t Button_Isr_Handle[eBUTTON_ID_NUMBEROFTYPE];

/*********************************************************/
/*********************************************************/
/*********************************************************/
Button_State_t Get_Button_State(Button_ID_t button_id)
{
    switch(button_id)
    {
        default:
        case eBUTTON_ID_1:
            if(HAL_GPIO_ReadPin(BUTTON_1_PORT, BUTTON_1_PIN) == BUTTON_1_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_2:
            if(HAL_GPIO_ReadPin(BUTTON_2_PORT, BUTTON_2_PIN) == BUTTON_2_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_3:
            if(HAL_GPIO_ReadPin(BUTTON_3_PORT, BUTTON_3_PIN) == BUTTON_3_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_4:
            if(HAL_GPIO_ReadPin(BUTTON_4_PORT, BUTTON_4_PIN) == BUTTON_4_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_5:
            if(HAL_GPIO_ReadPin(BUTTON_5_PORT, BUTTON_5_PIN) == BUTTON_5_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_6:
            if(HAL_GPIO_ReadPin(BUTTON_6_PORT, BUTTON_6_PIN) == BUTTON_6_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_7:
            if(HAL_GPIO_ReadPin(BUTTON_7_PORT, BUTTON_7_PIN) == BUTTON_7_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_8:
            if(HAL_GPIO_ReadPin(BUTTON_8_PORT, BUTTON_8_PIN) == BUTTON_8_ACTIVE)
            {
                return ePRESSED;
            }
            break;
        case eBUTTON_ID_9:
            if(HAL_GPIO_ReadPin(BUTTON_9_PORT, BUTTON_9_PIN) == BUTTON_9_ACTIVE)
            {
                return ePRESSED;
            }
            break;
    }
    return eRELEASED;
}
/*********************************************************/
void Button_1_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_1_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_1_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_1, Get_Button_State(eBUTTON_ID_1));	// goto callback functions
    }
}
/*********************************************************/
void Button_2_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_2_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_2_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_2, Get_Button_State(eBUTTON_ID_2));	// goto callback functions
    }
}
/*********************************************************/
void Button_3_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_3_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_3_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_3, Get_Button_State(eBUTTON_ID_3));	// goto callback functions
    }
}
/*********************************************************/
void Button_4_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_4_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_4_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_4, Get_Button_State(eBUTTON_ID_4));	// goto callback functions
    }
}
/*********************************************************/
void Button_5_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_5_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_5_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_5, Get_Button_State(eBUTTON_ID_5));	// goto callback functions
    }
}
/*********************************************************/
void Button_6_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_6_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_6_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_6, Get_Button_State(eBUTTON_ID_6));	// goto callback functions
    }
}
/*********************************************************/
void Button_7_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_7_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_7_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_7, Get_Button_State(eBUTTON_ID_7));	// goto callback functions
    }
}
/*********************************************************/
void Button_8_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_8_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_8_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_8, Get_Button_State(eBUTTON_ID_8));	// goto callback functions
    }
}
/*********************************************************/
void Button_9_ISR(void)
{
    if(__HAL_GPIO_EXTI_GET_IT(BUTTON_9_PIN) != RESET)
    {
        __HAL_GPIO_EXTI_CLEAR_IT(BUTTON_9_PIN);
        BUTTON_CALLBACK_FUNC(eBUTTON_ID_9, Get_Button_State(eBUTTON_ID_9));	// goto callback functions
    }
}
/*********************************************************/
void Buttons_Hw_Init(Button_ID_t button_id)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;

    switch(button_id)
    {
        case eBUTTON_ID_1:
            GPIO_InitStruct.Pin = BUTTON_1_PIN;
            GPIO_InitStruct.Pull = BUTTON_1_PULL;
            HAL_GPIO_Init(BUTTON_1_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_1_PORT, BUTTON_1_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_1], Button_1_ISR, BUTTON_1_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_1_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_1_ISR_ADR);
            break;
        case eBUTTON_ID_2:
            GPIO_InitStruct.Pin = BUTTON_2_PIN;
            GPIO_InitStruct.Pull = BUTTON_2_PULL;
            HAL_GPIO_Init(BUTTON_2_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_2_PORT, BUTTON_2_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_2], Button_2_ISR, BUTTON_2_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_2_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_2_ISR_ADR);
            break;
        case eBUTTON_ID_3:
            GPIO_InitStruct.Pin = BUTTON_3_PIN;
            GPIO_InitStruct.Pull = BUTTON_3_PULL;
            HAL_GPIO_Init(BUTTON_3_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_3_PORT, BUTTON_3_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_3], Button_3_ISR, BUTTON_3_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_3_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_3_ISR_ADR);
            break;
        case eBUTTON_ID_4:
            GPIO_InitStruct.Pin = BUTTON_4_PIN;
            GPIO_InitStruct.Pull = BUTTON_4_PULL;
            HAL_GPIO_Init(BUTTON_4_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_4_PORT, BUTTON_4_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_4], Button_4_ISR, BUTTON_4_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_4_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_4_ISR_ADR);
            break;
        case eBUTTON_ID_5:
            GPIO_InitStruct.Pin = BUTTON_5_PIN;
            GPIO_InitStruct.Pull = BUTTON_5_PULL;
            HAL_GPIO_Init(BUTTON_5_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_5_PORT, BUTTON_5_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_5], Button_5_ISR, BUTTON_5_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_5_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_5_ISR_ADR);
            break;
        case eBUTTON_ID_6:
            GPIO_InitStruct.Pin = BUTTON_6_PIN;
            GPIO_InitStruct.Pull = BUTTON_6_PULL;
            HAL_GPIO_Init(BUTTON_6_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_6_PORT, BUTTON_6_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_6], Button_6_ISR, BUTTON_6_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_6_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_6_ISR_ADR);
            break;
        case eBUTTON_ID_7:
            GPIO_InitStruct.Pin = BUTTON_7_PIN;
            GPIO_InitStruct.Pull = BUTTON_7_PULL;
            HAL_GPIO_Init(BUTTON_7_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_7_PORT, BUTTON_7_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_7], Button_7_ISR, BUTTON_7_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_7_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_7_ISR_ADR);
            break;
        case eBUTTON_ID_8:
            GPIO_InitStruct.Pin = BUTTON_8_PIN;
            GPIO_InitStruct.Pull = BUTTON_8_PULL;
            HAL_GPIO_Init(BUTTON_8_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_8_PORT, BUTTON_8_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_8], Button_8_ISR, BUTTON_8_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_8_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_8_ISR_ADR);
            break;
        case eBUTTON_ID_9:
            GPIO_InitStruct.Pin = BUTTON_9_PIN;
            GPIO_InitStruct.Pull = BUTTON_9_PULL;
            HAL_GPIO_Init(BUTTON_9_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_9_PORT, BUTTON_9_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_9], Button_9_ISR, BUTTON_9_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_9_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_9_ISR_ADR);
            break;
        case eBUTTON_ID_ALL:
            GPIO_InitStruct.Pin = BUTTON_1_PIN;
            GPIO_InitStruct.Pull = BUTTON_1_PULL;
            HAL_GPIO_Init(BUTTON_1_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_1_PORT, BUTTON_1_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_2_PIN;
            GPIO_InitStruct.Pull = BUTTON_2_PULL;
            HAL_GPIO_Init(BUTTON_2_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_2_PORT, BUTTON_2_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_3_PIN;
            GPIO_InitStruct.Pull = BUTTON_3_PULL;
            HAL_GPIO_Init(BUTTON_3_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_3_PORT, BUTTON_3_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_4_PIN;
            GPIO_InitStruct.Pull = BUTTON_4_PULL;
            HAL_GPIO_Init(BUTTON_4_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_4_PORT, BUTTON_4_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_5_PIN;
            GPIO_InitStruct.Pull = BUTTON_5_PULL;
            HAL_GPIO_Init(BUTTON_5_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_5_PORT, BUTTON_5_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_6_PIN;
            GPIO_InitStruct.Pull = BUTTON_6_PULL;
            HAL_GPIO_Init(BUTTON_6_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_6_PORT, BUTTON_6_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_7_PIN;
            GPIO_InitStruct.Pull = BUTTON_7_PULL;
            HAL_GPIO_Init(BUTTON_7_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_7_PORT, BUTTON_7_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_8_PIN;
            GPIO_InitStruct.Pull = BUTTON_8_PULL;
            HAL_GPIO_Init(BUTTON_8_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_8_PORT, BUTTON_8_PIN, GPIO_PIN_RESET);
            GPIO_InitStruct.Pin = BUTTON_9_PIN;
            GPIO_InitStruct.Pull = BUTTON_9_PULL;
            HAL_GPIO_Init(BUTTON_9_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(BUTTON_9_PORT, BUTTON_9_PIN, GPIO_PIN_RESET);

            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_1], Button_1_ISR, BUTTON_1_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_1_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_1_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_2], Button_2_ISR, BUTTON_2_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_2_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_2_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_3], Button_3_ISR, BUTTON_3_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_3_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_3_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_4], Button_4_ISR, BUTTON_4_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_4_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_4_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_5], Button_5_ISR, BUTTON_5_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_5_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_5_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_6], Button_6_ISR, BUTTON_6_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_6_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_6_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_7], Button_7_ISR, BUTTON_7_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_7_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_7_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_8], Button_8_ISR, BUTTON_8_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_8_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_8_ISR_ADR);
            SysIsr_Add(&Button_Isr_Handle[eBUTTON_ID_9], Button_9_ISR, BUTTON_9_ISR_HANDLER);
            HAL_NVIC_SetPriority(BUTTON_9_ISR_ADR, BUTTON_PREEMPTPRIORITY, BUTTON_SUBPRIORITY);
            HAL_NVIC_EnableIRQ(BUTTON_9_ISR_ADR);
            break;
        default:
            break;
    }
}
/*********************************************************/
void Buttons_Hw_DeInit(Button_ID_t button_id)
{
    switch(button_id)
    {
        case eBUTTON_ID_1:
            HAL_GPIO_DeInit(BUTTON_1_PORT, BUTTON_1_PIN);
            break;
        case eBUTTON_ID_2:
            HAL_GPIO_DeInit(BUTTON_2_PORT, BUTTON_2_PIN);
            break;
        case eBUTTON_ID_3:
            HAL_GPIO_DeInit(BUTTON_3_PORT, BUTTON_3_PIN);
            break;
        case eBUTTON_ID_4:
            HAL_GPIO_DeInit(BUTTON_4_PORT, BUTTON_4_PIN);
            break;
        case eBUTTON_ID_5:
            HAL_GPIO_DeInit(BUTTON_5_PORT, BUTTON_5_PIN);
            break;
        case eBUTTON_ID_6:
            HAL_GPIO_DeInit(BUTTON_6_PORT, BUTTON_6_PIN);
            break;
        case eBUTTON_ID_7:
            HAL_GPIO_DeInit(BUTTON_7_PORT, BUTTON_7_PIN);
            break;
        case eBUTTON_ID_8:
            HAL_GPIO_DeInit(BUTTON_8_PORT, BUTTON_8_PIN);
            break;
        case eBUTTON_ID_9:
            HAL_GPIO_DeInit(BUTTON_9_PORT, BUTTON_9_PIN);
            break;
        case eBUTTON_ID_ALL:
            HAL_GPIO_DeInit(BUTTON_1_PORT, BUTTON_1_PIN);
            HAL_GPIO_DeInit(BUTTON_2_PORT, BUTTON_2_PIN);
            HAL_GPIO_DeInit(BUTTON_3_PORT, BUTTON_3_PIN);
            HAL_GPIO_DeInit(BUTTON_4_PORT, BUTTON_4_PIN);
            HAL_GPIO_DeInit(BUTTON_5_PORT, BUTTON_5_PIN);
            HAL_GPIO_DeInit(BUTTON_6_PORT, BUTTON_6_PIN);
            HAL_GPIO_DeInit(BUTTON_7_PORT, BUTTON_7_PIN);
            HAL_GPIO_DeInit(BUTTON_8_PORT, BUTTON_8_PIN);
            HAL_GPIO_DeInit(BUTTON_9_PORT, BUTTON_9_PIN);
            break;
        default:
            break;
    }
}
/*********************************************************/
void Buttons_Hw_Enable(Button_ID_t button_id)
{
    switch(button_id)
    {
        case eBUTTON_ID_1:
            HAL_NVIC_EnableIRQ(BUTTON_1_ISR_ADR);
            break;
        case eBUTTON_ID_2:
            HAL_NVIC_EnableIRQ(BUTTON_2_ISR_ADR);
            break;
        case eBUTTON_ID_3:
            HAL_NVIC_EnableIRQ(BUTTON_3_ISR_ADR);
            break;
        case eBUTTON_ID_4:
            HAL_NVIC_EnableIRQ(BUTTON_4_ISR_ADR);
            break;
        case eBUTTON_ID_5:
            HAL_NVIC_EnableIRQ(BUTTON_5_ISR_ADR);
            break;
        case eBUTTON_ID_6:
            HAL_NVIC_EnableIRQ(BUTTON_6_ISR_ADR);
            break;
        case eBUTTON_ID_7:
            HAL_NVIC_EnableIRQ(BUTTON_7_ISR_ADR);
            break;
        case eBUTTON_ID_8:
            HAL_NVIC_EnableIRQ(BUTTON_8_ISR_ADR);
            break;
        case eBUTTON_ID_9:
            HAL_NVIC_EnableIRQ(BUTTON_9_ISR_ADR);
            break;
        case eBUTTON_ID_ALL:
            HAL_NVIC_EnableIRQ(BUTTON_1_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_2_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_3_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_4_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_5_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_6_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_7_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_8_ISR_ADR);
            HAL_NVIC_EnableIRQ(BUTTON_9_ISR_ADR);
            break;
        default:
            break;
    }
}
/*********************************************************/
void Buttons_Hw_Disable(Button_ID_t button_id)
{
    switch(button_id)
    {
        case eBUTTON_ID_1:
            HAL_NVIC_DisableIRQ(BUTTON_1_ISR_ADR);
            break;
        case eBUTTON_ID_2:
            HAL_NVIC_DisableIRQ(BUTTON_2_ISR_ADR);
            break;
        case eBUTTON_ID_3:
            HAL_NVIC_DisableIRQ(BUTTON_3_ISR_ADR);
            break;
        case eBUTTON_ID_4:
            HAL_NVIC_DisableIRQ(BUTTON_4_ISR_ADR);
            break;
        case eBUTTON_ID_5:
            HAL_NVIC_DisableIRQ(BUTTON_5_ISR_ADR);
            break;
        case eBUTTON_ID_6:
            HAL_NVIC_DisableIRQ(BUTTON_6_ISR_ADR);
            break;
        case eBUTTON_ID_7:
            HAL_NVIC_DisableIRQ(BUTTON_7_ISR_ADR);
            break;
        case eBUTTON_ID_8:
            HAL_NVIC_DisableIRQ(BUTTON_8_ISR_ADR);
            break;
        case eBUTTON_ID_9:
            HAL_NVIC_DisableIRQ(BUTTON_9_ISR_ADR);
            break;
        case eBUTTON_ID_ALL:
            HAL_NVIC_DisableIRQ(BUTTON_1_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_2_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_3_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_4_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_5_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_6_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_7_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_8_ISR_ADR);
            HAL_NVIC_DisableIRQ(BUTTON_9_ISR_ADR);
            break;
        default:
            break;
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/

