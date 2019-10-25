/*
 *  Glass Lcd Library
 *
 *  Created on: Oct 22, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef LIB_GLASSLCD_H_
#define LIB_GLASSLCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

typedef enum
{
    LCD_DIGIT_POSITION_1 = 0,
    LCD_DIGIT_POSITION_2 = 1,
    LCD_DIGIT_POSITION_3 = 2,
    LCD_DIGIT_POSITION_4 = 3,
    LCD_DIGIT_POSITION_5 = 4,
    LCD_DIGIT_POSITION_6 = 5,
    LCD_DIGIT_POSITION_7 = 6,
    LCD_DIGIT_POSITION_8 = 7,
    LCD_DIGIT_POSITION_9 = 8,
    LCD_DIGIT_POSITION_10 = 9,
    LCD_DIGIT_POSITION_11 = 10,
    LCD_DIGIT_POSITION_12 = 11,
    LCD_DIGIT_POSITION_13 = 12,
    LCD_DIGIT_MAX_NUMBER = 13,
} Glasslcd_DigitPosition_t;

typedef enum
{
    eGLASSLCD_DOUBLE,
    eGLASSLCD_LONGDOUBLE,
    eGLASSLCD_DECIMAL,
    eGLASSLCD_STRING,
    eGLASSLCD_SET_ICON,
    eGLASSLCD_RESET_ICON,
    eGLASSLCD_SET_ALL_ICON,
    eGLASSLCD_RESET_ALL_ICON,
    eGLASSLCD_CLEAR,
    eGLASSLCD_SET_CONTRAST
} Glasslcd_Command_t;

typedef enum
{
    eICON_COMMA,
    eICON_UP_POINT_1,
    eICON_UP_POINT_2,
    eICON_UP_POINT_3,
    eICON_BOX,
    eICON_UP_KW,
    eICON_HOUR,
    eICON_UP_M3,
    eICON_UP_LT,
    eICON_ARROW_HIGH,
    eICON_ARROW_LINE,
    eICON_ARROW_LOW,
    eICON_DN_POINT_1,
    eICON_DN_POINT_2,
    eICON_DN_POINT_3,
    eICON_DN_KW,
    eICON_DN_LTH,
    eICON_DN_M3H,
    eICON_ANTENNA,
    eICON_BATTERY,
    eICON_BATTERY_1,
    eICON_BATTERY_2,
    eICON_BATTERY_3,
    eICON_VALVE,
    eICON_FAULT,
    eICON_WARNING,
    eICON_LEAKEGE,
    eICON_SIZEOF,
} Glasslcd_Icon_t;

typedef struct
{
    Glasslcd_Command_t command;
    Glasslcd_Icon_t icon;
    double double_value;
    int32_t decimal_value;
    uint8_t minus;
    char string[13];
} Glasslcd_Struct_t;

typedef enum
{
    eGLASSLCD_DATA_FRAME,
    eGLASSLCD_CONFIG_FRAME,
    eGLASSLCD_NUMBEROFTYPE
} Glasslcd_Frame_t;

typedef enum
{
    eGLASSLCD_INIT,
    eGLASSLCD_DEINIT,
    eGLASSLCD_DISABLE,
    eGLASSLCD_ENABLE,
    eGLASSLCD_NUMBEROFTYPES
} Glasslcd_Config_t;

typedef struct
{
    Glasslcd_Frame_t frame_type;
    Glasslcd_Config_t config;
} Glasslcd_Config_Frame_t;

typedef struct
{
    Glasslcd_Frame_t frame_type;
    Glasslcd_Struct_t lcd;
} Glasslcd_Data_Frame_t;


#define SEGMEND_A (digit & 0x1)
#define SEGMEND_B ((digit & 0x2) >> 1)
#define SEGMEND_C ((digit & 0x4) >> 2)
#define SEGMEND_D ((digit & 0x8) >> 3)
#define SEGMEND_E ((digit & 0x10) >> 4)
#define SEGMEND_F ((digit & 0x20) >> 5)
#define SEGMEND_G ((digit & 0x40) >> 6)

#define MINUS_CHAR 16
#define R_CHAR  	17
#define NULL_CHAR 18
#define L_CHAR  	19
#define T_CHAR  	20
#define n_CHAR  	21
#define o_CHAR  	22
#define Y_CHAR  	23
#define H_CHAR  	24
#define I_CHAR  	25
#define EQUAL_CHAR  	26

#define G_CHAR  	9
#define S_CHAR  	5

#define A_CHAR  	0xA
#define B_CHAR  	0xB
#define C_CHAR  	0xC
#define D_CHAR  	0xD
#define E_CHAR  	0xE
#define F_CHAR  	0xF

osStatus_t SendDataMsg_Glasslcd(Glasslcd_Struct_t *data);
osStatus_t SendConfigMsg_Glasslcd(Glasslcd_Config_t config);

#ifdef __cplusplus
}
#endif

#endif /* __LIB_GLASSLCD_H */
