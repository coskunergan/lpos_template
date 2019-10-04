/*
 *  Pin Defines
 *
 *  Created on: Sep 30, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef PIN_DEFINES_H_
#define PIN_DEFINES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

//--------------------------------
#define LED_1_PIN  	GPIO_PIN_12 
#define LED_1_PORT 	GPIOC
	
#define LED_2_PIN  	GPIO_PIN_12 
#define LED_2_PORT 	GPIOC
	
#define LED_3_PIN  	GPIO_PIN_12 
#define LED_3_PORT 	GPIOC

#define LED_4_PIN  	GPIO_PIN_12 
#define LED_4_PORT 	GPIOC

#define LED_5_PIN  	GPIO_PIN_12 
#define LED_5_PORT 	GPIOC
	
#define LED_6_PIN  	GPIO_PIN_12 
#define LED_6_PORT 	GPIOC

#define LED_7_PIN  	GPIO_PIN_12 
#define LED_7_PORT 	GPIOC

#define LED_8_PIN  	GPIO_PIN_12 
#define LED_8_PORT 	GPIOC

#define LED_9_PIN  	GPIO_PIN_12 
#define LED_9_PORT 	GPIOC
//--------------------------------
#define BUZZER_PIN  	GPIO_PIN_13 
#define BUZZER_PORT 	GPIOC
//--------------------------------
#define BUTTON_1_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_1_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_1_PIN  				GPIO_PIN_10
#define BUTTON_1_PORT 				GPIOC
#define BUTTON_1_PULL   			GPIO_PULLUP
#define BUTTON_1_ACTIVE     	RESET
	
#define BUTTON_2_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_2_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_2_PIN  				GPIO_PIN_10
#define BUTTON_2_PORT 				GPIOC
#define BUTTON_2_PULL   			GPIO_PULLUP
#define BUTTON_2_ACTIVE     	RESET
	
#define BUTTON_3_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_3_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_3_PIN  				GPIO_PIN_10
#define BUTTON_3_PORT 				GPIOC
#define BUTTON_3_PULL   			GPIO_PULLUP
#define BUTTON_3_ACTIVE     	RESET

#define BUTTON_4_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_4_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_4_PIN  				GPIO_PIN_10
#define BUTTON_4_PORT 				GPIOC
#define BUTTON_4_PULL   			GPIO_PULLUP
#define BUTTON_4_ACTIVE     	RESET

#define BUTTON_5_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_5_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_5_PIN  				GPIO_PIN_10
#define BUTTON_5_PORT 				GPIOC
#define BUTTON_5_PULL   			GPIO_PULLUP
#define BUTTON_5_ACTIVE     	RESET

#define BUTTON_6_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_6_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_6_PIN  				GPIO_PIN_10
#define BUTTON_6_PORT 				GPIOC
#define BUTTON_6_PULL   			GPIO_PULLUP
#define BUTTON_6_ACTIVE     	RESET

#define BUTTON_7_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_7_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_7_PIN  				GPIO_PIN_10
#define BUTTON_7_PORT 				GPIOC
#define BUTTON_7_PULL   			GPIO_PULLUP
#define BUTTON_7_ACTIVE     	RESET

#define BUTTON_8_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_8_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_8_PIN  				GPIO_PIN_10
#define BUTTON_8_PORT 				GPIOC
#define BUTTON_8_PULL   			GPIO_PULLUP
#define BUTTON_8_ACTIVE     	RESET

#define BUTTON_9_ISR_HANDLER  _EXTI15_10_IRQHandler
#define BUTTON_9_ISR_ADR   		EXTI15_10_IRQn
#define BUTTON_9_PIN  				GPIO_PIN_10
#define BUTTON_9_PORT 				GPIOC
#define BUTTON_9_PULL   			GPIO_PULLUP
#define BUTTON_9_ACTIVE     	RESET
//--------------------------------
#ifdef __cplusplus
}
#endif

#endif /* PIN_DEFINES_H_ */
