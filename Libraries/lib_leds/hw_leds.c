/*
 *  Leds Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_leds.h"
#include "pin_defines.h"

/*********************************************************/
void Leds_Hw_Init(Led_ID_t led_id)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    switch(led_id)
    {
        case eLED_ID_1:
            GPIO_InitStruct.Pin = LED_1_PIN;
            HAL_GPIO_Init(LED_1_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, LED_1_PASSIVE);
            break;
        case eLED_ID_2:
            GPIO_InitStruct.Pin = LED_2_PIN;
            HAL_GPIO_Init(LED_2_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, LED_2_PASSIVE);
            break;
        case eLED_ID_3:
            GPIO_InitStruct.Pin = LED_3_PIN;
            HAL_GPIO_Init(LED_3_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, LED_3_PASSIVE);
            break;
        case eLED_ID_4:
            GPIO_InitStruct.Pin = LED_4_PIN;
            HAL_GPIO_Init(LED_4_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, LED_4_PASSIVE);
            break;
        case eLED_ID_5:
            GPIO_InitStruct.Pin = LED_5_PIN;
            HAL_GPIO_Init(LED_5_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, LED_5_PASSIVE);
            break;
        case eLED_ID_6:
            GPIO_InitStruct.Pin = LED_6_PIN;
            HAL_GPIO_Init(LED_6_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, LED_6_PASSIVE);
            break;
        case eLED_ID_7:
            GPIO_InitStruct.Pin = LED_7_PIN;
            HAL_GPIO_Init(LED_7_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, LED_7_PASSIVE);
            break;
        case eLED_ID_8:
            GPIO_InitStruct.Pin = LED_8_PIN;
            HAL_GPIO_Init(LED_8_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, LED_8_PASSIVE);
            break;
        case eLED_ID_9:
            GPIO_InitStruct.Pin = LED_9_PIN;
            HAL_GPIO_Init(LED_9_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, LED_9_PASSIVE);
            break;
        case eLED_ID_ALL:
            GPIO_InitStruct.Pin = LED_1_PIN;
            HAL_GPIO_Init(LED_1_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, LED_1_PASSIVE);
            GPIO_InitStruct.Pin = LED_2_PIN;
            HAL_GPIO_Init(LED_2_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, LED_2_PASSIVE);
            GPIO_InitStruct.Pin = LED_3_PIN;
            HAL_GPIO_Init(LED_3_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, LED_3_PASSIVE);
            GPIO_InitStruct.Pin = LED_4_PIN;
            HAL_GPIO_Init(LED_4_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, LED_4_PASSIVE);
            GPIO_InitStruct.Pin = LED_5_PIN;
            HAL_GPIO_Init(LED_5_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, LED_5_PASSIVE);
            GPIO_InitStruct.Pin = LED_6_PIN;
            HAL_GPIO_Init(LED_6_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, LED_6_PASSIVE);
            GPIO_InitStruct.Pin = LED_7_PIN;
            HAL_GPIO_Init(LED_7_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, LED_7_PASSIVE);
            GPIO_InitStruct.Pin = LED_8_PIN;
            HAL_GPIO_Init(LED_8_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, LED_8_PASSIVE);
            GPIO_InitStruct.Pin = LED_9_PIN;
            HAL_GPIO_Init(LED_9_PORT, &GPIO_InitStruct);
            HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, LED_9_PASSIVE);
            break;
        default:
            break;
    }
}
/*********************************************************/
void Leds_Hw_DeInit(Led_ID_t led_id)
{
    switch(led_id)
    {
        case eLED_ID_1:
            HAL_GPIO_DeInit(LED_1_PORT, LED_1_PIN);
            break;
        case eLED_ID_2:
            HAL_GPIO_DeInit(LED_2_PORT, LED_2_PIN);
            break;
        case eLED_ID_3:
            HAL_GPIO_DeInit(LED_3_PORT, LED_3_PIN);
            break;
        case eLED_ID_4:
            HAL_GPIO_DeInit(LED_4_PORT, LED_4_PIN);
            break;
        case eLED_ID_5:
            HAL_GPIO_DeInit(LED_5_PORT, LED_5_PIN);
            break;
        case eLED_ID_6:
            HAL_GPIO_DeInit(LED_6_PORT, LED_6_PIN);
            break;
        case eLED_ID_7:
            HAL_GPIO_DeInit(LED_7_PORT, LED_7_PIN);
            break;
        case eLED_ID_8:
            HAL_GPIO_DeInit(LED_8_PORT, LED_8_PIN);
            break;
        case eLED_ID_9:
            HAL_GPIO_DeInit(LED_9_PORT, LED_9_PIN);
            break;
        case eLED_ID_ALL:
            HAL_GPIO_DeInit(LED_1_PORT, LED_1_PIN);
            HAL_GPIO_DeInit(LED_2_PORT, LED_2_PIN);
            HAL_GPIO_DeInit(LED_3_PORT, LED_3_PIN);
            HAL_GPIO_DeInit(LED_4_PORT, LED_4_PIN);
            HAL_GPIO_DeInit(LED_5_PORT, LED_5_PIN);
            HAL_GPIO_DeInit(LED_6_PORT, LED_6_PIN);
            HAL_GPIO_DeInit(LED_7_PORT, LED_7_PIN);
            HAL_GPIO_DeInit(LED_8_PORT, LED_8_PIN);
            HAL_GPIO_DeInit(LED_9_PORT, LED_9_PIN);
            break;
        default:
            break;
    }
}
/*********************************************************/
void Led_Operation(Led_Data_Frame_t *data_msg)
{
    if(data_msg->state == eLED_ON)
    {
        switch(data_msg->led_id)
        {
            case eLED_ID_1:
                HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, LED_1_ACTIVE);
                break;
            case eLED_ID_2:
                HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, LED_2_ACTIVE);
                break;
            case eLED_ID_3:
                HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, LED_3_ACTIVE);
                break;
            case eLED_ID_4:
                HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, LED_4_ACTIVE);
                break;
            case eLED_ID_5:
                HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, LED_5_ACTIVE);
                break;
            case eLED_ID_6:
                HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, LED_6_ACTIVE);
                break;
            case eLED_ID_7:
                HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, LED_7_ACTIVE);
                break;
            case eLED_ID_8:
                HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, LED_8_ACTIVE);
                break;
            case eLED_ID_9:
                HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, LED_9_ACTIVE);
                break;
            default:
                break;
        }
    }
    else if(data_msg->state == eLED_OFF)
    {
        switch(data_msg->led_id)
        {
            case eLED_ID_1:
                HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, LED_1_PASSIVE);
                break;
            case eLED_ID_2:
                HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, LED_2_PASSIVE);
                break;
            case eLED_ID_3:
                HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, LED_3_PASSIVE);
                break;
            case eLED_ID_4:
                HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, LED_4_PASSIVE);
                break;
            case eLED_ID_5:
                HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, LED_5_PASSIVE);
                break;
            case eLED_ID_6:
                HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, LED_6_PASSIVE);
                break;
            case eLED_ID_7:
                HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, LED_7_PASSIVE);
                break;
            case eLED_ID_8:
                HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, LED_8_PASSIVE);
                break;
            case eLED_ID_9:
                HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, LED_9_PASSIVE);
                break;
            default:
                break;
        }
    }
    else if(data_msg->state == eLED_TOGGLE)
    {
        switch(data_msg->led_id)
        {
            case eLED_ID_1:
                HAL_GPIO_TogglePin(LED_1_PORT, LED_1_PIN);
                break;
            case eLED_ID_2:
                HAL_GPIO_TogglePin(LED_2_PORT, LED_2_PIN);
                break;
            case eLED_ID_3:
                HAL_GPIO_TogglePin(LED_3_PORT, LED_3_PIN);
                break;
            case eLED_ID_4:
                HAL_GPIO_TogglePin(LED_4_PORT, LED_4_PIN);
                break;
            case eLED_ID_5:
                HAL_GPIO_TogglePin(LED_5_PORT, LED_5_PIN);
                break;
            case eLED_ID_6:
                HAL_GPIO_TogglePin(LED_6_PORT, LED_6_PIN);
                break;
            case eLED_ID_7:
                HAL_GPIO_TogglePin(LED_7_PORT, LED_7_PIN);
                break;
            case eLED_ID_8:
                HAL_GPIO_TogglePin(LED_8_PORT, LED_8_PIN);
                break;
            case eLED_ID_9:
                HAL_GPIO_TogglePin(LED_9_PORT, LED_9_PIN);
                break;
            default:
                break;
        }
    }
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
