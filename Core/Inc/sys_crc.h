/*
 *  CRC Port Operation
 *
 *  Created on: Sep 28, 2019
 *
 *  Author: Coskun ERGAN
 */

#ifndef SYS_CRC_H_
#define SYS_CRC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void SysCrc_Init(void);
uint16_t SysCrc_Part(const uint8_t *nData, uint16_t wLength);
uint16_t SysCrc_Data(const uint8_t *nData, uint16_t wLength);

#ifdef __cplusplus
}
#endif

#endif /* SYS_CRC_H_ */
