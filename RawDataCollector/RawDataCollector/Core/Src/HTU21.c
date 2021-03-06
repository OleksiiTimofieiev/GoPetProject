/*
 * HTU21.c
 *
 *  Created on: Oct 1, 2021
 *      Author: otimofieiev
 */


#include "HTU21.h"

void readTempAndHumidity(I2C_HandleTypeDef *hi2c1, DeviceData *deviceData) {
	uint8_t HTU21D_RX_Data[2];

	uint16_t HTU21D_ADC_Raw;

	HAL_I2C_Mem_Read(hi2c1, HTU21D_Adress, HTU21D_Temp_Cmd, I2C_MEMADD_SIZE_8BIT, (uint8_t*) HTU21D_RX_Data, 2, 1000);
	HTU21D_ADC_Raw = ((uint16_t)(HTU21D_RX_Data[0] << 8) | (HTU21D_RX_Data[1]));
	deviceData->HTU21D_Temperature = (int)(HTU21D_ADC_Raw * 175.72 / 65536.00) - 46.85;
	HAL_Delay(100);

	HAL_I2C_Mem_Read(hi2c1, HTU21D_Adress, HTU21D_Humi_Cmd, I2C_MEMADD_SIZE_8BIT, HTU21D_RX_Data, 2, 1000);
	HTU21D_ADC_Raw = ((uint16_t)(HTU21D_RX_Data[0] << 8) | (HTU21D_RX_Data[1]));
	deviceData->HTU21D_Humidity = (int)(HTU21D_ADC_Raw * 125.0 / 65536.0) - 6.0;
}
