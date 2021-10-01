/*
 * MPU6050.h
 *
 *  Created on: Oct 1, 2021
 *      Author: otimofieiev
 */

#ifndef INC_HTU21_H_
#define INC_HTU21_H_

#include "stm32f4xx_hal.h"

#define HTU21D_Adress	( 0x40 << 1 )
#define HTU21D_Temp_Cmd ( 0xE3 )
#define HTU21D_Humi_Cmd ( 0xE5 )

void readTempAndHumidity(I2C_HandleTypeDef *hi2c1);

#endif /* INC_HTU21_H_ */
