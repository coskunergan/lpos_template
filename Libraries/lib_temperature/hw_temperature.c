/*
 *  Temperature Library
 *
 *  Created on: Oct 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_temperature.h"
#include "pin_defines.h"

#ifdef LIB_TEMPERATURE

static uint8_t AdcInitialized = 0;

static ADC_HandleTypeDef hadc;

/*********************************************************/
void Temperature_Hw_Init(void)
{
    ADC_ChannelConfTypeDef   sConfig = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    if(AdcInitialized == 0)
    {
        __HAL_RCC_ADC_CLK_ENABLE();

        hadc.Instance = ADC1;

        if(HAL_ADC_DeInit(&hadc) != HAL_OK)
        {
            vAssertCalled(__FILE__, __LINE__);
        }

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
        PeriphClkInit.AdcClockSelection = RCC_ADCCLKSOURCE_SYSCLK;
        if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
        {
            vAssertCalled(__FILE__, __LINE__);
        }

        hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
        hadc.Init.Resolution = ADC_RESOLUTION_12B;
        hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
        hadc.Init.ScanConvMode = ADC_SCAN_DISABLE;
        hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
        hadc.Init.LowPowerAutoWait = DISABLE;
        hadc.Init.ContinuousConvMode = DISABLE;
        hadc.Init.NbrOfConversion = 1;
        hadc.Init.DiscontinuousConvMode = DISABLE;
        hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
        hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
        hadc.Init.DMAContinuousRequests = DISABLE;
        hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
        hadc.Init.OversamplingMode = DISABLE;
        if(HAL_ADC_Init(&hadc) != HAL_OK)
        {
            vAssertCalled(__FILE__, __LINE__);
        }

        sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
        sConfig.Rank = ADC_REGULAR_RANK_1;
        sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        sConfig.SingleDiff = ADC_SINGLE_ENDED;
        sConfig.OffsetNumber = ADC_OFFSET_NONE;
        sConfig.Offset = 0;
        if(HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
        {
            vAssertCalled(__FILE__, __LINE__);
        }
        AdcInitialized = 1;
    }
}
/*********************************************************/
void Temperature_Hw_DeInit(void)
{
    if(AdcInitialized == 1)
    {
        HAL_ADC_DeInit(&hadc);
        AdcInitialized = 0;
    }
}
/*********************************************************/
int8_t Temperature_Read(void)
{
    ADC_ChannelConfTypeDef adcConf = {0};
    uint16_t measuredLevel;
    uint32_t VoltageLevelmV;

    if(AdcInitialized == 1)
    {
        __HAL_RCC_ADC_CLK_ENABLE();

        if(HAL_ADCEx_Calibration_Start(&hadc, ADC_SINGLE_ENDED) !=  HAL_OK)
        {
            /* ADC Calibration Error */
            vAssertCalled(__FILE__, __LINE__);
        }

        /* configure adc channel */
        adcConf.Channel = ADC_CHANNEL_VREFINT;
        adcConf.Rank = ADC_REGULAR_RANK_1;
        adcConf.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
        adcConf.SingleDiff = ADC_SINGLE_ENDED;
        adcConf.OffsetNumber = ADC_OFFSET_NONE;
        adcConf.Offset = 0;
        if(HAL_ADC_ConfigChannel(&hadc, &adcConf) != HAL_OK)
        {
            vAssertCalled(__FILE__, __LINE__);
        }

        /* Start the conversion process */
        HAL_ADC_Start(&hadc);

        /* Wait for the end of conversion */
        HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

        /* Get the converted value of regular channel */
        measuredLevel = HAL_ADC_GetValue(&hadc);

        ADC_Disable(&hadc);

        VoltageLevelmV = ((uint32_t) VREFINT_CAL_VREF * (*VREFINT_CAL_ADDR)) / measuredLevel;

        /* configure adc channel */
        adcConf.Channel = ADC_CHANNEL_TEMPSENSOR;
        adcConf.Rank = ADC_REGULAR_RANK_1;
        adcConf.SamplingTime = ADC_SAMPLETIME_12CYCLES_5;
        adcConf.SingleDiff = ADC_SINGLE_ENDED;
        adcConf.OffsetNumber = ADC_OFFSET_NONE;
        adcConf.Offset = 0;
        if(HAL_ADC_ConfigChannel(&hadc, &adcConf) != HAL_OK)
        {
            vAssertCalled(__FILE__, __LINE__);
        }

        /* Start the conversion process */
        HAL_ADC_Start(&hadc);

        /* Wait for the end of conversion */
        HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);

        /* Get the converted value of regular channel */
        measuredLevel = HAL_ADC_GetValue(&hadc);

        ADC_Disable(&hadc);

        __HAL_RCC_ADC_CLK_DISABLE();

        return __LL_ADC_CALC_TEMPERATURE(VoltageLevelmV, measuredLevel, LL_ADC_RESOLUTION_12B);
    }
    return 0;
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
#endif
