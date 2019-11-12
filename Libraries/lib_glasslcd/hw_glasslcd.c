/*
 *  Glass Lcd Library
 *
 *  Created on: Oct 22, 2019
 *
 *  Author: Coskun ERGAN
 */

#include "lib_glasslcd.h"
#include "pin_defines.h"
#include "stdio.h"
#include "stdlib.h"

const uint32_t glasslcd_icon_table[eICON_SIZEOF][2] =
{
    {(1UL << ICONBIT_COMMA),  ICONREG_COMMA 			},     //eICON_COMMA
    {(1UL << ICONBIT_UP_POINT_1),  ICONREG_UP_POINT_1 	},  //eICON_UP_POINT_1
    {(1UL << ICONBIT_UP_POINT_2),  ICONREG_UP_POINT_2	},  //eICON_UP_POINT_2
    {(1UL << ICONBIT_UP_POINT_3),  ICONREG_UP_POINT_3 	},  //eICON_UP_POINT_3
    {(1UL << ICONBIT_BOX),  ICONREG_BOX 				},       //eICON_BOX
    {(1UL << ICONBIT_UP_KW),  ICONREG_UP_KW 			},     //eICON_UP_KW
    {(1UL << ICONBIT_HOUR),  ICONREG_HOUR 				},     //eICON_HOUR
    {(1UL << ICONBIT_UP_M3),  ICONREG_UP_M3 			},     //eICON_UP_M3
    {(1UL << ICONBIT_UP_LT),  ICONREG_UP_LT 			},     //eICON_UP_LT
    {(1UL << ICONBIT_ARROW_HIGH),  ICONREG_ARROW_HIGH 	},  //eICON_ARROW_HIGH
    {(1UL << ICONBIT_ARROW_LINE),  ICONREG_ARROW_LINE 	},  //eICON_ARROW_LINE
    {(1UL << ICONBIT_ARROW_LOW),  ICONREG_ARROW_LOW 	},   //eICON_ARROW_LOW
    {(1UL << ICONBIT_DN_POINT_1),  ICONREG_DN_POINT_1	},  //eICON_DN_POINT_1
    {(1UL << ICONBIT_DN_POINT_2),  ICONREG_DN_POINT_2 	},  //eICON_DN_POINT_2
    {(1UL << ICONBIT_DN_POINT_3),  ICONREG_DN_POINT_3 	},  //eICON_DN_POINT_3
    {(1UL << ICONBIT_DN_KW),  ICONREG_DN_KW 			},     //eICON_DN_KW
    {(1UL << ICONBIT_DN_LTH),  ICONREG_DN_LTH 			},    //eICON_DN_LTH
    {(1UL << ICONBIT_DN_M3H),  ICONREG_DN_M3H 			},    //eICON_DN_M3H
    {(1UL << ICONBIT_ANTENNA),  ICONREG_ANTENNA 		},    //eICON_ANTENNA
    {(1UL << ICONBIT_BATTERY),  ICONREG_BATTERY 		},    //eICON_BATTERY
    {(1UL << ICONBIT_BATTERY_1),  ICONREG_BATTERY_1 	},   //eICON_BATTERY_1
    {(1UL << ICONBIT_BATTERY_2),  ICONREG_BATTERY_2 	},   //eICON_BATTERY_2
    {(1UL << ICONBIT_BATTERY_3),  ICONREG_BATTERY_3 	},   //eICON_BATTERY_3
    {(1UL << ICONBIT_VALVE),  ICONREG_VALVE 			},     //eICON_VALVE
    {(1UL << ICONBIT_FAULT),  ICONREG_FAULT 			},     //eICON_FAULT
    {(1UL << ICONBIT_WARNING),  ICONREG_WARNING 		},    //eICON_WARNING
    {(1UL << ICONBIT_LEAKEGE),  ICONREG_LEAKEGE     }      //eICON_LEAKEGE
};

static LCD_HandleTypeDef LCDHandle;

void Glasslcd_IconChange(uint8_t icon , uint8_t state);
void Glasslcd_IconChangeAll(uint8_t state);
void Glasslcd_WriteStruct(Glasslcd_Struct_t *lcd);
void Glasslcd_Hw_Init(void);
void Glasslcd_Hw_DeInit(void);
static void WriteDigit(uint8_t digit, Glasslcd_DigitPosition_t Position);
static void LCD_Write(LCD_HandleTypeDef *hlcd, uint32_t RAMRegisterIndex, uint32_t RAMRegisterMask, uint32_t Data);
static void LCD_UpdateDisplayRequest(LCD_HandleTypeDef *hlcd);
/*********************************************************/
static void BSP_LCD_GLASS_Contrast(uint32_t Contrast)
{
    __HAL_LCD_CONTRAST_CONFIG(&LCDHandle, Contrast);
}
/*********************************************************/
static void BSP_LCD_GLASS_Display_Double(double number, uint8_t useBottomScreen)
{
    uint8_t buff[16];

    sprintf((char *)buff, "% f", number);// "-0,123456"
    sprintf((char *)&buff[8], "%02d", (uint32_t)(number * 10000000) % 100); // "-0,123456"

    if(buff[2] == '.')//+1.123456
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        if(buff[0] == '-')
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_5);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_5);
        }
        WriteDigit(buff[1] - '0', LCD_DIGIT_POSITION_6);
        WriteDigit(buff[3] - '0', LCD_DIGIT_POSITION_7);
        WriteDigit(buff[4] - '0', LCD_DIGIT_POSITION_8);
        WriteDigit(buff[5] - '0', LCD_DIGIT_POSITION_9);
        if(useBottomScreen)
        {
            WriteDigit(buff[6] - '0', LCD_DIGIT_POSITION_10);
            WriteDigit(buff[7] - '0', LCD_DIGIT_POSITION_11);
            WriteDigit(buff[8] - '0', LCD_DIGIT_POSITION_12);
            WriteDigit(buff[9] - '0', LCD_DIGIT_POSITION_13);
        }
    }

    else if(buff[3] == '.')
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        if(buff[0] == '-')
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_4);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        }
        WriteDigit(buff[1] - '0', LCD_DIGIT_POSITION_5);
        WriteDigit(buff[2] - '0', LCD_DIGIT_POSITION_6);
        WriteDigit(buff[4] - '0', LCD_DIGIT_POSITION_7);
        WriteDigit(buff[5] - '0', LCD_DIGIT_POSITION_8);
        WriteDigit(buff[6] - '0', LCD_DIGIT_POSITION_9);
        if(useBottomScreen)
        {
            WriteDigit(buff[7] - '0', LCD_DIGIT_POSITION_10);
            WriteDigit(buff[8] - '0', LCD_DIGIT_POSITION_11);
            WriteDigit(buff[9] - '0', LCD_DIGIT_POSITION_12);
            WriteDigit(buff[10] - '0', LCD_DIGIT_POSITION_13);
        }
    }
    else if(buff[4] == '.')
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        if(buff[0] == '-')
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_3);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        }
        WriteDigit(buff[1] - '0', LCD_DIGIT_POSITION_4);
        WriteDigit(buff[2] - '0', LCD_DIGIT_POSITION_5);
        WriteDigit(buff[3] - '0', LCD_DIGIT_POSITION_6);
        WriteDigit(buff[5] - '0', LCD_DIGIT_POSITION_7);
        WriteDigit(buff[6] - '0', LCD_DIGIT_POSITION_8);
        WriteDigit(buff[7] - '0', LCD_DIGIT_POSITION_9);
        if(useBottomScreen)
        {
            WriteDigit(buff[8] - '0', LCD_DIGIT_POSITION_10);
            WriteDigit(buff[9] - '0', LCD_DIGIT_POSITION_11);
            WriteDigit(buff[10] - '0', LCD_DIGIT_POSITION_12);
            WriteDigit(buff[11] - '0', LCD_DIGIT_POSITION_13);
        }
    }
    else if(buff[5] == '.')
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        if(buff[0] == '-')
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_2);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        }
        WriteDigit(buff[1] - '0', LCD_DIGIT_POSITION_3);
        WriteDigit(buff[2] - '0', LCD_DIGIT_POSITION_4);
        WriteDigit(buff[3] - '0', LCD_DIGIT_POSITION_5);
        WriteDigit(buff[4] - '0', LCD_DIGIT_POSITION_6);
        WriteDigit(buff[6] - '0', LCD_DIGIT_POSITION_7);
        WriteDigit(buff[7] - '0', LCD_DIGIT_POSITION_8);
        WriteDigit(buff[8] - '0', LCD_DIGIT_POSITION_9);
        if(useBottomScreen)
        {
            WriteDigit(buff[9] - '0', LCD_DIGIT_POSITION_10);
            WriteDigit(buff[10] - '0', LCD_DIGIT_POSITION_11);
            WriteDigit(buff[11] - '0', LCD_DIGIT_POSITION_12);
            WriteDigit(buff[12] - '0', LCD_DIGIT_POSITION_13);
        }
    }
    else if(buff[6] == '.')
    {
        if(buff[0] == '-')
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_1);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        }
        WriteDigit(buff[1] - '0', LCD_DIGIT_POSITION_2);
        WriteDigit(buff[2] - '0', LCD_DIGIT_POSITION_3);
        WriteDigit(buff[3] - '0', LCD_DIGIT_POSITION_4);
        WriteDigit(buff[4] - '0', LCD_DIGIT_POSITION_5);
        WriteDigit(buff[5] - '0', LCD_DIGIT_POSITION_6);
        WriteDigit(buff[7] - '0', LCD_DIGIT_POSITION_7);
        WriteDigit(buff[8] - '0', LCD_DIGIT_POSITION_8);
        WriteDigit(buff[9] - '0', LCD_DIGIT_POSITION_9);
        if(useBottomScreen)
        {
            WriteDigit(buff[10] - '0', LCD_DIGIT_POSITION_10);
            WriteDigit(buff[11] - '0', LCD_DIGIT_POSITION_11);
            WriteDigit(buff[12] - '0', LCD_DIGIT_POSITION_12);
            WriteDigit(buff[13] - '0', LCD_DIGIT_POSITION_13);
        }
    }

    Glasslcd_IconChange(eICON_COMMA, 0);
    Glasslcd_IconChange(eICON_UP_POINT_1, 1);
    Glasslcd_IconChange(eICON_UP_POINT_2, 0);
    Glasslcd_IconChange(eICON_UP_POINT_3, 0);
    if(useBottomScreen)
    {
        Glasslcd_IconChange(eICON_DN_POINT_1, 0);
        Glasslcd_IconChange(eICON_DN_POINT_2, 0);
        Glasslcd_IconChange(eICON_DN_POINT_3, 0);
    }
}
/*********************************************************/
static void BSP_LCD_GLASS_Display_String(char *str)
{
    uint8_t i;
    for(i = 0; i < 13; i++)
    {
        switch(*str)
        {
            case '0':
                *str = 0;
                break;
            case '1':
                *str = 1;
                break;
            case '2':
                *str = 2;
                break;
            case '3':
                *str = 3;
                break;
            case '4':
                *str = 4;
                break;
            case '5':
                *str = 5;
                break;
            case '6':
                *str = 6;
                break;
            case '7':
                *str = 7;
                break;
            case '8':
                *str = 8;
                break;
            case '9':
                *str = 9;
                break;
            case '-':
                *str = MINUS_CHAR;
                break;
            case 'A':
                *str = A_CHAR;
                break;
            case 'B':
                *str = B_CHAR;
                break;
            case 'C':
                *str = C_CHAR;
                break;
            case 'D':
                *str = D_CHAR;
                break;
            case 'E':
                *str = E_CHAR;
                break;
            case 'F':
                *str = F_CHAR;
                break;
            case 'R':
                *str = R_CHAR;
                break;
            case 'L':
                *str = L_CHAR;
                break;
            case 'T':
                *str = T_CHAR;
                break;
            case 'N':
                *str = n_CHAR;
                break;
            case 'O':
                *str = o_CHAR;
                break;
            case 'Y':
                *str = Y_CHAR;
                break;
            case 'H':
                *str = H_CHAR;
                break;
            case 'I':
                *str = I_CHAR;
                break;
            case 'G':
                *str = G_CHAR;
                break;
            case 'S':
                *str = S_CHAR;
                break;
            case 'a':
                *str = A_CHAR;
                break;
            case 'b':
                *str = B_CHAR;
                break;
            case 'c':
                *str = C_CHAR;
                break;
            case 'd':
                *str = D_CHAR;
                break;
            case 'e':
                *str = E_CHAR;
                break;
            case 'f':
                *str = F_CHAR;
                break;
            case 'r':
                *str = R_CHAR;
                break;
            case 'l':
                *str = L_CHAR;
                break;
            case 't':
                *str = T_CHAR;
                break;
            case 'n':
                *str = n_CHAR;
                break;
            case 'o':
                *str = o_CHAR;
                break;
            case 'y':
                *str = Y_CHAR;
                break;
            case 'h':
                *str = H_CHAR;
                break;
            case 'i':
                *str = I_CHAR;
                break;
            case 'g':
                *str = G_CHAR;
                break;
            case 's':
                *str = S_CHAR;
                break;
            case '=':
                *str = EQUAL_CHAR;
                break;
            case '\'':
                *str = DEGREE_CHAR;
                break;						
            default:
                *str = NULL_CHAR;
                break;
        }
        WriteDigit(*str++, (Glasslcd_DigitPosition_t)(LCD_DIGIT_POSITION_1 + i));
    }
}
/*********************************************************/
static void BSP_LCD_GLASS_Display_Decimal(int32_t value)
{
    uint32_t number = (value < 0) ? value * (-1) : value;
    uint8_t ch1 = number % 10;
    uint8_t ch2 = (number / 10) % 10;
    uint8_t ch3 = (number / 100) % 10;
    uint8_t ch4 = (number / 1000) % 10;
    uint8_t ch5 = (number / 10000) % 10;
    uint8_t ch6 = (number / 100000) % 10;
    uint8_t ch7 = (number / 1000000) % 10;
    uint8_t ch8 = (number / 10000000) % 10;
    uint8_t ch9 = (number / 100000000) % 10;

    if(ch9)
    {
        WriteDigit(ch9, LCD_DIGIT_POSITION_1);
        WriteDigit(ch8, LCD_DIGIT_POSITION_2);
        WriteDigit(ch7, LCD_DIGIT_POSITION_3);
        WriteDigit(ch6, LCD_DIGIT_POSITION_4);
        WriteDigit(ch5, LCD_DIGIT_POSITION_5);
        WriteDigit(ch4, LCD_DIGIT_POSITION_6);
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch8)
    {
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_1);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        }
        WriteDigit(ch8, LCD_DIGIT_POSITION_2);
        WriteDigit(ch7, LCD_DIGIT_POSITION_3);
        WriteDigit(ch6, LCD_DIGIT_POSITION_4);
        WriteDigit(ch5, LCD_DIGIT_POSITION_5);
        WriteDigit(ch4, LCD_DIGIT_POSITION_6);
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch7)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_2);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        }
        WriteDigit(ch7, LCD_DIGIT_POSITION_3);
        WriteDigit(ch6, LCD_DIGIT_POSITION_4);
        WriteDigit(ch5, LCD_DIGIT_POSITION_5);
        WriteDigit(ch4, LCD_DIGIT_POSITION_6);
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch6)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_3);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        }
        WriteDigit(ch6, LCD_DIGIT_POSITION_4);
        WriteDigit(ch5, LCD_DIGIT_POSITION_5);
        WriteDigit(ch4, LCD_DIGIT_POSITION_6);
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch5)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_4);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        }
        WriteDigit(ch5, LCD_DIGIT_POSITION_5);
        WriteDigit(ch4, LCD_DIGIT_POSITION_6);
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch4)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_5);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_5);
        }
        WriteDigit(ch4, LCD_DIGIT_POSITION_6);
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch3)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_5);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_6);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_6);
        }
        WriteDigit(ch3, LCD_DIGIT_POSITION_7);
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch2)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_5);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_6);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_7);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_7);
        }
        WriteDigit(ch2, LCD_DIGIT_POSITION_8);
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }
    else if(ch1)
    {
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_1);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_2);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_3);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_4);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_5);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_6);
        WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_7);
        if(value < 0)
        {
            WriteDigit(MINUS_CHAR, LCD_DIGIT_POSITION_8);
        }
        else
        {
            WriteDigit(NULL_CHAR, LCD_DIGIT_POSITION_8);
        }
        WriteDigit(ch1, LCD_DIGIT_POSITION_9);
    }

    Glasslcd_IconChange(eICON_COMMA, 1);
    Glasslcd_IconChange(eICON_UP_POINT_1, 1);
    Glasslcd_IconChange(eICON_UP_POINT_2, 1);
    Glasslcd_IconChange(eICON_UP_POINT_3, 1);
}
/*********************************************************/
static void BSP_LCD_GLASS_Clear(void)
{
    HAL_LCD_Clear(&LCDHandle);
}
/*********************************************************/
static void LCD_MspInit(LCD_HandleTypeDef *hlcd)
{
    extern void Error_Handler(void);

    GPIO_InitTypeDef  gpioinitstruct = {0};
    RCC_OscInitTypeDef oscinitstruct = {0};
    RCC_PeriphCLKInitTypeDef periphclkstruct = {0};

    /*##-1- Enable PWR  peripheral Clock #######################################*/
    __HAL_RCC_PWR_CLK_ENABLE();

    /*##-2- Configure LSE as RTC clock soucre ###################################*/
    oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_LSE;
    oscinitstruct.PLL.PLLState    = RCC_PLL_NONE;
    oscinitstruct.LSEState        = RCC_LSE_ON;
    if(HAL_RCC_OscConfig(&oscinitstruct) != HAL_OK)
    {
        Error_Handler();
    }

    /*##-3- Select LSE as RTC clock source.##########################*/
    /* Backup domain management is done in RCC function */
    periphclkstruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    periphclkstruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    HAL_RCCEx_PeriphCLKConfig(&periphclkstruct);

    /*##-4- Enable LCD GPIO Clocks #############################################*/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();


    /*##-5- Configure peripheral GPIO ##########################################*/
    /* Configure Output for LCD */
    /* Port A */
    gpioinitstruct.Pin        = LCD_GPIO_BANKA_PINS;
    gpioinitstruct.Mode       = GPIO_MODE_AF_PP;
    gpioinitstruct.Pull       = GPIO_NOPULL;
    gpioinitstruct.Speed      = GPIO_SPEED_FREQ_VERY_HIGH;
    gpioinitstruct.Alternate  = GPIO_AF11_LCD;
    HAL_GPIO_Init(GPIOA, &gpioinitstruct);

    /* Port B */
    gpioinitstruct.Pin        = LCD_GPIO_BANKB_PINS;
    HAL_GPIO_Init(GPIOB, &gpioinitstruct);

    /* Port C*/
    gpioinitstruct.Pin        = LCD_GPIO_BANKC_PINS;
    HAL_GPIO_Init(GPIOC, &gpioinitstruct);

    /* Port D */
    gpioinitstruct.Pin        = LCD_GPIO_BANKD_PINS;
    HAL_GPIO_Init(GPIOD, &gpioinitstruct);

    /* Port E */
    gpioinitstruct.Pin        = LCD_GPIO_BANKE_PINS;
    HAL_GPIO_Init(GPIOE, &gpioinitstruct);

    /* Wait for the external capacitor Cext which is connected to the VLCD pin is charged
    (approximately 2ms for Cext=1uF) */

    //HAL_Delay(2);

    /*##-6- Enable LCD peripheral Clock ########################################*/
    __HAL_RCC_LCD_CLK_ENABLE();
}
/*********************************************************/
static void LCD_MspDeInit(LCD_HandleTypeDef *hlcd)
{
    uint32_t gpiopin = 0;

    /*##-1- Enable LCD GPIO Clocks #############################################*/
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    /*##-1- Configure peripheral GPIO ##########################################*/
    /* Configure Output for LCD */
    /* Port A */
    gpiopin = LCD_GPIO_BANKA_PINS;
    HAL_GPIO_DeInit(GPIOA, gpiopin);

    /* Port B */
    gpiopin = LCD_GPIO_BANKB_PINS;
    HAL_GPIO_DeInit(GPIOB, gpiopin);

    /* Port C*/
    gpiopin = LCD_GPIO_BANKC_PINS;
    HAL_GPIO_DeInit(GPIOC, gpiopin);

    /* Port D */
    gpiopin = LCD_GPIO_BANKD_PINS;
    HAL_GPIO_DeInit(GPIOD, gpiopin);

    /*##-5- Enable LCD peripheral Clock ########################################*/
    __HAL_RCC_LCD_CLK_DISABLE();
}
/*********************************************************/
static void LCD_Write(LCD_HandleTypeDef *hlcd, uint32_t RAMRegisterIndex, uint32_t RAMRegisterMask, uint32_t Data)
{
    /* Copy the new Data bytes to LCD RAM register */
    MODIFY_REG(hlcd->Instance->RAM[RAMRegisterIndex], ~(RAMRegisterMask), Data);
    //HAL_LCD_Write(hlcd,RAMRegisterIndex,RAMRegisterMask,Data);
}
/*********************************************************/
static void LCD_UpdateDisplayRequest(LCD_HandleTypeDef *hlcd)
{
    /* Clear the Update Display Done flag before starting the update display request */
    __HAL_LCD_CLEAR_FLAG(hlcd, LCD_FLAG_UDD);

    /* Enable the display request */
    hlcd->Instance->SR |= LCD_SR_UDR;
    //HAL_LCD_UpdateDisplayRequest(hlcd);
    //osDelay(100);
}
/*********************************************************/
static void WriteDigit(uint8_t digit, Glasslcd_DigitPosition_t Position)
{
    const uint8_t number_table[28] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71, 0x40, 0x50, 0x00, 0x38, 0x78, 0x54, 0x5C, 0x6E, 0x76, 0x04, 0x48, 0x63}; //,0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F,-,r,NULL,L,T,n,o,y,h,i,=,°
    uint32_t data = 0x00;
    uint32_t mask = 0x00;

    if(digit >= sizeof(number_table))
    {
        digit = NULL_CHAR;
    }
    digit = number_table[digit]; // convert

    switch(Position)
    {
        /* Position 1 on LCD (Digit1)*/
        case LCD_DIGIT_POSITION_1:
            data = (SEGMEND_A << LCD_DIGIT_1A_BIT);
            mask = (1 << LCD_DIGIT_1A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_1B_BIT);
            mask = (1 << LCD_DIGIT_1B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_1C_BIT);
            mask = (1 << LCD_DIGIT_1C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_1D_BIT);
            mask = (1 << LCD_DIGIT_1D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_1E_BIT);
            mask = (1 << LCD_DIGIT_1E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_1F_BIT);
            mask = (1 << LCD_DIGIT_1F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_1G_BIT);
            mask = (1 << LCD_DIGIT_1G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_1G_REG, ~mask, data);
            break;

        /* Position 2 on LCD (Digit2)*/
        case LCD_DIGIT_POSITION_2:
            data = (SEGMEND_A << LCD_DIGIT_2A_BIT);
            mask = (1 << LCD_DIGIT_2A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_2B_BIT);
            mask = (1 << LCD_DIGIT_2B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_2C_BIT);
            mask = (1 << LCD_DIGIT_2C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_2D_BIT);
            mask = (1 << LCD_DIGIT_2D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_2E_BIT);
            mask = (1 << LCD_DIGIT_2E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_2F_BIT);
            mask = (1 << LCD_DIGIT_2F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_2G_BIT);
            mask = (1 << LCD_DIGIT_2G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_2G_REG, ~mask, data);
            break;

        /* Position 3 on LCD (Digit3)*/
        case LCD_DIGIT_POSITION_3:
            data = (SEGMEND_A << LCD_DIGIT_3A_BIT);
            mask = (1 << LCD_DIGIT_3A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_3B_BIT);
            mask = (1 << LCD_DIGIT_3B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_3C_BIT);
            mask = (1 << LCD_DIGIT_3C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_3D_BIT);
            mask = (1 << LCD_DIGIT_3D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_3E_BIT);
            mask = (1 << LCD_DIGIT_3E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_3F_BIT);
            mask = (1 << LCD_DIGIT_3F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_3G_BIT);
            mask = (1 << LCD_DIGIT_3G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_3G_REG, ~mask, data);
            break;

        /* Position 4 on LCD (Digit4)*/
        case LCD_DIGIT_POSITION_4:
            data = (SEGMEND_A << LCD_DIGIT_4A_BIT);
            mask = (1 << LCD_DIGIT_4A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_4B_BIT);
            mask = (1 << LCD_DIGIT_4B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_4C_BIT);
            mask = (1 << LCD_DIGIT_4C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_4D_BIT);
            mask = (1 << LCD_DIGIT_4D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_4E_BIT);
            mask = (1 << LCD_DIGIT_4E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_4F_BIT);
            mask = (1 << LCD_DIGIT_4F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_4G_BIT);
            mask = (1 << LCD_DIGIT_4G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_4G_REG, ~mask, data);
            break;

        /* Position 5 on LCD (Digit5)*/
        case LCD_DIGIT_POSITION_5:
            data = (SEGMEND_A << LCD_DIGIT_5A_BIT);
            mask = (1 << LCD_DIGIT_5A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_5B_BIT);
            mask = (1 << LCD_DIGIT_5B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_5C_BIT);
            mask = (1 << LCD_DIGIT_5C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_5D_BIT);
            mask = (1 << LCD_DIGIT_5D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_5E_BIT);
            mask = (1 << LCD_DIGIT_5E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_5F_BIT);
            mask = (1 << LCD_DIGIT_5F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_5G_BIT);
            mask = (1 << LCD_DIGIT_5G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_5G_REG, ~mask, data);
            break;

        /* Position 6 on LCD (Digit6)*/
        case LCD_DIGIT_POSITION_6:
            data = (SEGMEND_A << LCD_DIGIT_6A_BIT);
            mask = (1 << LCD_DIGIT_6A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_6B_BIT);
            mask = (1 << LCD_DIGIT_6B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_6C_BIT);
            mask = (1 << LCD_DIGIT_6C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_6D_BIT);
            mask = (1 << LCD_DIGIT_6D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_6E_BIT);
            mask = (1 << LCD_DIGIT_6E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_6F_BIT);
            mask = (1 << LCD_DIGIT_6F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_6G_BIT);
            mask = (1 << LCD_DIGIT_6G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_6G_REG, ~mask, data);
            break;

        /* Position 7 on LCD (Digit7)*/
        case LCD_DIGIT_POSITION_7:
            data = (SEGMEND_A << LCD_DIGIT_7A_BIT);
            mask = (1 << LCD_DIGIT_7A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_7B_BIT);
            mask = (1 << LCD_DIGIT_7B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_7C_BIT);
            mask = (1 << LCD_DIGIT_7C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_7D_BIT);
            mask = (1 << LCD_DIGIT_7D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_7E_BIT);
            mask = (1 << LCD_DIGIT_7E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_7F_BIT);
            mask = (1 << LCD_DIGIT_7F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_7G_BIT);
            mask = (1 << LCD_DIGIT_7G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_7G_REG, ~mask, data);
            break;
        /* Position 8 on LCD (Digit8)*/
        case LCD_DIGIT_POSITION_8:
            data = (SEGMEND_A << LCD_DIGIT_8A_BIT);
            mask = (1 << LCD_DIGIT_8A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_8B_BIT);
            mask = (1 << LCD_DIGIT_8B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_8C_BIT);
            mask = (1 << LCD_DIGIT_8C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_8D_BIT);
            mask = (1 << LCD_DIGIT_8D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_8E_BIT);
            mask = (1 << LCD_DIGIT_8E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_8F_BIT);
            mask = (1 << LCD_DIGIT_8F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_8G_BIT);
            mask = (1 << LCD_DIGIT_8G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_8G_REG, ~mask, data);
            break;
        /* Position 9 on LCD (Digit9)*/
        case LCD_DIGIT_POSITION_9:
            data = (SEGMEND_A << LCD_DIGIT_9A_BIT);
            mask = (1 << LCD_DIGIT_9A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_9B_BIT);
            mask = (1 << LCD_DIGIT_9B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_9C_BIT);
            mask = (1 << LCD_DIGIT_9C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_9D_BIT);
            mask = (1 << LCD_DIGIT_9D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_9E_BIT);
            mask = (1 << LCD_DIGIT_9E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_9F_BIT);
            mask = (1 << LCD_DIGIT_9F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_9G_BIT);
            mask = (1 << LCD_DIGIT_9G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_9G_REG, ~mask, data);
            break;
        /* Position 10 on LCD (Digit10)*/
        case LCD_DIGIT_POSITION_10:
            data = (SEGMEND_A << LCD_DIGIT_10A_BIT);
            mask = (1 << LCD_DIGIT_10A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_10B_BIT);
            mask = (1 << LCD_DIGIT_10B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_10C_BIT);
            mask = (1 << LCD_DIGIT_10C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_10D_BIT);
            mask = (1 << LCD_DIGIT_10D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_10E_BIT);
            mask = (1 << LCD_DIGIT_10E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_10F_BIT);
            mask = (1 << LCD_DIGIT_10F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_10G_BIT);
            mask = (1 << LCD_DIGIT_10G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_10G_REG, ~mask, data);
            break;
        /* Position 11 on LCD (Digit11)*/
        case LCD_DIGIT_POSITION_11:
            data = (SEGMEND_A << LCD_DIGIT_11A_BIT);
            mask = (1 << LCD_DIGIT_11A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_11B_BIT);
            mask = (1 << LCD_DIGIT_11B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_11C_BIT);
            mask = (1 << LCD_DIGIT_11C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_11D_BIT);
            mask = (1 << LCD_DIGIT_11D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_11E_BIT);
            mask = (1 << LCD_DIGIT_11E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_11F_BIT);
            mask = (1 << LCD_DIGIT_11F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_11G_BIT);
            mask = (1 << LCD_DIGIT_11G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_11G_REG, ~mask, data);
            break;
        /* Position 12 on LCD (Digit12)*/
        case LCD_DIGIT_POSITION_12:
            data = (SEGMEND_A << LCD_DIGIT_12A_BIT);
            mask = (1 << LCD_DIGIT_12A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_12B_BIT);
            mask = (1 << LCD_DIGIT_12B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_12C_BIT);
            mask = (1 << LCD_DIGIT_12C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_12D_BIT);
            mask = (1 << LCD_DIGIT_12D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_12E_BIT);
            mask = (1UL << LCD_DIGIT_12E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_12F_BIT);
            mask = (1UL << LCD_DIGIT_12F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_12G_BIT);
            mask = (1UL << LCD_DIGIT_12G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_12G_REG, ~mask, data);
            break;
        /* Position 13 on LCD (Digit13)*/
        case LCD_DIGIT_POSITION_13:
            data = (SEGMEND_A << LCD_DIGIT_13A_BIT);
            mask = (1 << LCD_DIGIT_13A_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13A_REG, ~mask, data);

            data = (SEGMEND_B << LCD_DIGIT_13B_BIT);
            mask = (1 << LCD_DIGIT_13B_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13B_REG, ~mask, data);

            data = (SEGMEND_C << LCD_DIGIT_13C_BIT);
            mask = (1 << LCD_DIGIT_13C_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13C_REG, ~mask, data);

            data = (SEGMEND_D << LCD_DIGIT_13D_BIT);
            mask = (1 << LCD_DIGIT_13D_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13D_REG, ~mask, data);

            data = (SEGMEND_E << LCD_DIGIT_13E_BIT);
            mask = (1 << LCD_DIGIT_13E_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13E_REG, ~mask, data);

            data = (SEGMEND_F << LCD_DIGIT_13F_BIT);
            mask = (1 << LCD_DIGIT_13F_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13F_REG, ~mask, data);

            data = (SEGMEND_G << LCD_DIGIT_13G_BIT);
            mask = (1 << LCD_DIGIT_13G_BIT);
            LCD_Write(&LCDHandle, LCD_DIGIT_13G_REG, ~mask, data);
            break;
        default:
            break;
    }
}
/***************************************************************************************/
void Glasslcd_IconChange(uint8_t icon , uint8_t state)
{
    if(state == 0)
    {
        LCD_Write(&LCDHandle, glasslcd_icon_table[icon][1], ~glasslcd_icon_table[icon][0], 0);// icon OFF
    }
    else
    {
        LCD_Write(&LCDHandle, glasslcd_icon_table[icon][1], ~glasslcd_icon_table[icon][0], glasslcd_icon_table[icon][0]);// icon ON
    }
}
/*********************************************************/
void Glasslcd_IconChangeAll(uint8_t state)
{
    uint8_t i;

    for(i = 0; i < eICON_SIZEOF; i++)
    {
        if(state == 0)
        {
            LCD_Write(&LCDHandle, glasslcd_icon_table[i][1], ~glasslcd_icon_table[i][0], 0);// icon OFF
        }
        else
        {
            LCD_Write(&LCDHandle, glasslcd_icon_table[i][1], ~glasslcd_icon_table[i][0], glasslcd_icon_table[i][0]);// icon ON
        }
    }
}
/*********************************************************/
void Glasslcd_WriteStruct(Glasslcd_Struct_t *lcd)
{
    switch(lcd->command)
    {
        case eGLASSLCD_DOUBLE:
            BSP_LCD_GLASS_Display_Double(lcd->double_value, 0);
            break;
        case eGLASSLCD_LONGDOUBLE:
            BSP_LCD_GLASS_Display_Double(lcd->double_value, 1);
            break;
        case eGLASSLCD_DECIMAL:
            BSP_LCD_GLASS_Display_Decimal(lcd->decimal_value);
            break;
        case eGLASSLCD_STRING:
            BSP_LCD_GLASS_Display_String(lcd->string);
            break;
        case eGLASSLCD_SET_ICON:
            Glasslcd_IconChange(lcd->icon, 1);
            break;
        case eGLASSLCD_RESET_ICON:
            Glasslcd_IconChange(lcd->icon, 0);
            break;
        case eGLASSLCD_SET_ALL_ICON:
            Glasslcd_IconChangeAll(1);
            break;
        case eGLASSLCD_RESET_ALL_ICON:
            Glasslcd_IconChangeAll(0);
            break;
        case eGLASSLCD_CLEAR:
            HAL_LCD_Clear(&LCDHandle);
            break;
        case eGLASSLCD_SET_CONTRAST:
            BSP_LCD_GLASS_Contrast(lcd->decimal_value);
            break;
        default:
            break;
    }
    LCD_UpdateDisplayRequest(&LCDHandle);
}
/*********************************************************/
void Glasslcd_Hw_Init(void)
{
    //lcd + stop2 current 5µA
//    LCDHandle.Instance              = LCD;
//    LCDHandle.Init.Prescaler        = LCD_PRESCALER_4;
//    LCDHandle.Init.Divider          = LCD_DIVIDER_20;
//    LCDHandle.Init.Duty             = LCD_DUTY_1_4;
//    LCDHandle.Init.Bias             = LCD_BIAS_1_4;
//    LCDHandle.Init.VoltageSource    = LCD_VOLTAGESOURCE_EXTERNAL;
//    LCDHandle.Init.Contrast         = LCD_CONTRASTLEVEL_7;
//    LCDHandle.Init.DeadTime         = LCD_DEADTIME_4;
//    LCDHandle.Init.PulseOnDuration  = LCD_PULSEONDURATION_0;
//    LCDHandle.Init.HighDrive        = LCD_HIGHDRIVE_ENABLE;
//    LCDHandle.Init.BlinkMode        = LCD_BLINKMODE_OFF;
//    LCDHandle.Init.BlinkFrequency   = LCD_BLINKFREQUENCY_DIV32;
//    LCDHandle.Init.MuxSegment       = LCD_MUXSEGMENT_DISABLE;

    //lcd + stop2 current 7µA
    LCDHandle.Instance              = LCD;
    LCDHandle.Init.Prescaler        = LCD_PRESCALER_4;
    LCDHandle.Init.Divider          = LCD_DIVIDER_31;
    LCDHandle.Init.Duty             = LCD_DUTY_1_4;
    LCDHandle.Init.Bias             = LCD_BIAS_1_3;
    LCDHandle.Init.VoltageSource    = LCD_VOLTAGESOURCE_EXTERNAL;
    LCDHandle.Init.Contrast         = LCD_CONTRASTLEVEL_4;
    LCDHandle.Init.DeadTime         = LCD_DEADTIME_2;
    LCDHandle.Init.PulseOnDuration  = LCD_PULSEONDURATION_2;
    LCDHandle.Init.HighDrive        = LCD_HIGHDRIVE_DISABLE;
    LCDHandle.Init.BlinkMode        = LCD_BLINKMODE_OFF;
    LCDHandle.Init.BlinkFrequency   = LCD_BLINKFREQUENCY_DIV32;
    LCDHandle.Init.MuxSegment       = LCD_MUXSEGMENT_DISABLE;

    //lcd + stop2 current 9µA
//    LCDHandle.Instance              = LCD;
//    LCDHandle.Init.Prescaler        = LCD_PRESCALER_4;
//    LCDHandle.Init.Divider          = LCD_DIVIDER_31;
//    LCDHandle.Init.Duty             = LCD_DUTY_1_4;
//    LCDHandle.Init.Bias             = LCD_BIAS_1_3;
//    LCDHandle.Init.VoltageSource    = LCD_VOLTAGESOURCE_EXTERNAL;
//    LCDHandle.Init.Contrast         = LCD_CONTRASTLEVEL_4;
//    LCDHandle.Init.DeadTime         = LCD_DEADTIME_0;
//    LCDHandle.Init.PulseOnDuration  = LCD_PULSEONDURATION_4;
//    LCDHandle.Init.HighDrive        = LCD_HIGHDRIVE_DISABLE;
//    LCDHandle.Init.BlinkMode        = LCD_BLINKMODE_OFF;
//    LCDHandle.Init.BlinkFrequency   = LCD_BLINKFREQUENCY_DIV32;
//    LCDHandle.Init.MuxSegment       = LCD_MUXSEGMENT_DISABLE;

    LCD_MspInit(&LCDHandle);
    HAL_LCD_Init(&LCDHandle);
    BSP_LCD_GLASS_Clear();
}
/*********************************************************/
void Glasslcd_Hw_DeInit(void)
{
    LCD_MspDeInit(&LCDHandle);
    HAL_LCD_DeInit(&LCDHandle);
}
/*********************************************************/
/*********************************************************/
/*********************************************************/
