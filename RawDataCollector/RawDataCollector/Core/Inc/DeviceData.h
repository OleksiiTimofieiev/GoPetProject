/*
 * data.h
 *
 *  Created on: Oct 2, 2021
 *      Author: otimofieiev
 */

#ifndef INC_DEVICEDATA_H_
#define INC_DEVICEDATA_H_

typedef struct	AllData
{
	float		HTU21D_Temperature;
	float		HTU21D_Humidity;
	float		BMP280_Pressure;
	uint16_t	BH1750_Lumens;

}				DeviceData;

#endif /* INC_DEVICEDATA_H_ */
