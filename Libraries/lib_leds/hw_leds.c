/*
 *  Leds Library
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_leds.h"
#include "hw_leds.h"
#include "pin_defines.h"

/*********************************************************/
void Leds_Hw_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    GPIO_InitStruct.Pin = LED_1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LED_1_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_2_PIN;
    HAL_GPIO_Init(LED_2_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_3_PIN;
    HAL_GPIO_Init(LED_3_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_4_PIN;
    HAL_GPIO_Init(LED_4_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_5_PIN;
    HAL_GPIO_Init(LED_5_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_6_PIN;
    HAL_GPIO_Init(LED_6_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_7_PIN;
    HAL_GPIO_Init(LED_7_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_8_PIN;
    HAL_GPIO_Init(LED_8_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin = LED_9_PIN;
    HAL_GPIO_Init(LED_9_PORT, &GPIO_InitStruct);
    HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, GPIO_PIN_RESET);
}
/*********************************************************/
void Leds_Hw_DeInit(void)
{
    HAL_GPIO_DeInit(LED_1_PORT, LED_1_PIN);
    HAL_GPIO_DeInit(LED_2_PORT, LED_2_PIN);
    HAL_GPIO_DeInit(LED_3_PORT, LED_3_PIN);
    HAL_GPIO_DeInit(LED_4_PORT, LED_4_PIN);
    HAL_GPIO_DeInit(LED_5_PORT, LED_5_PIN);
    HAL_GPIO_DeInit(LED_6_PORT, LED_6_PIN);
    HAL_GPIO_DeInit(LED_7_PORT, LED_7_PIN);
    HAL_GPIO_DeInit(LED_8_PORT, LED_8_PIN);
    HAL_GPIO_DeInit(LED_9_PORT, LED_9_PIN);
}
/*********************************************************/
void Led_Operation(Led_Data_Frame_t *data_msg)
{
    if(data_msg->state == eON)
    {
        switch(data_msg->led_id)
        {
            case eLED_1_ID:
                HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_SET);
                break;
            case eLED_2_ID:
                HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_SET);
                break;
            case eLED_3_ID:
                HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_SET);
                break;
            case eLED_4_ID:
                HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_SET);
                break;
            case eLED_5_ID:
                HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, GPIO_PIN_SET);
                break;
            case eLED_6_ID:
                HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, GPIO_PIN_SET);
                break;
            case eLED_7_ID:
                HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, GPIO_PIN_SET);
                break;
            case eLED_8_ID:
                HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, GPIO_PIN_SET);
                break;
            case eLED_9_ID:
                HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, GPIO_PIN_SET);
                break;
            default:
                break;
        }
    }
    else if(data_msg->state == eOFF)
    {
        switch(data_msg->led_id)
        {
            case eLED_1_ID:
                HAL_GPIO_WritePin(LED_1_PORT, LED_1_PIN, GPIO_PIN_RESET);
                break;
            case eLED_2_ID:
                HAL_GPIO_WritePin(LED_2_PORT, LED_2_PIN, GPIO_PIN_RESET);
                break;
            case eLED_3_ID:
                HAL_GPIO_WritePin(LED_3_PORT, LED_3_PIN, GPIO_PIN_RESET);
                break;
            case eLED_4_ID:
                HAL_GPIO_WritePin(LED_4_PORT, LED_4_PIN, GPIO_PIN_RESET);
                break;
            case eLED_5_ID:
                HAL_GPIO_WritePin(LED_5_PORT, LED_5_PIN, GPIO_PIN_RESET);
                break;
            case eLED_6_ID:
                HAL_GPIO_WritePin(LED_6_PORT, LED_6_PIN, GPIO_PIN_RESET);
                break;
            case eLED_7_ID:
                HAL_GPIO_WritePin(LED_7_PORT, LED_7_PIN, GPIO_PIN_RESET);
                break;
            case eLED_8_ID:
                HAL_GPIO_WritePin(LED_8_PORT, LED_8_PIN, GPIO_PIN_RESET);
                break;
            case eLED_9_ID:
                HAL_GPIO_WritePin(LED_9_PORT, LED_9_PIN, GPIO_PIN_RESET);
                break;
            default:
                break;
        }
    }
    else if(data_msg->state == eTOGGLE)
    {
        switch(data_msg->led_id)
        {
            case eLED_1_ID:
                HAL_GPIO_TogglePin(LED_1_PORT, LED_1_PIN);
                break;
            case eLED_2_ID:
                HAL_GPIO_TogglePin(LED_2_PORT, LED_2_PIN);
                break;
            case eLED_3_ID:
                HAL_GPIO_TogglePin(LED_3_PORT, LED_3_PIN);
                break;
            case eLED_4_ID:
                HAL_GPIO_TogglePin(LED_4_PORT, LED_4_PIN);
                break;
            case eLED_5_ID:
                HAL_GPIO_TogglePin(LED_5_PORT, LED_5_PIN);
                break;
            case eLED_6_ID:
                HAL_GPIO_TogglePin(LED_6_PORT, LED_6_PIN);
                break;
            case eLED_7_ID:
                HAL_GPIO_TogglePin(LED_7_PORT, LED_7_PIN);
                break;
            case eLED_8_ID:
                HAL_GPIO_TogglePin(LED_8_PORT, LED_8_PIN);
                break;
            case eLED_9_ID:
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
