/*
 * BH1750.c
 *
 *  Created on: Oct 1, 2021
 *      Author: otimofieiev
 */


/*
 * BH1750.h
 *
 *  Created on: Sep 30, 2021
 *      Author: otimofieiev
 */

#include "BH1750.h"

#include <stdlib.h>
#include <string.h>


//HAL_StatusTypeDef BH1750_get_lumen(BH1750_device_t* dev);

void _Error_Handler(char * file, int line)
{
  while(1)
  {
  }
}

//HAL_StatusTypeDef BH1750_init_i2c(I2C_HandleTypeDef* i2c_handle)
//{
//	  __HAL_RCC_GPIOB_CLK_ENABLE();
//
//	i2c_handle->Instance = I2C1;
//	i2c_handle->Init.ClockSpeed = 100000;
//	i2c_handle->Init.DutyCycle = I2C_DUTYCYCLE_2;
//	i2c_handle->Init.OwnAddress1 = 0;
//	i2c_handle->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
//	i2c_handle->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
//	i2c_handle->Init.OwnAddress2 = 0;
//	i2c_handle->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
//	i2c_handle->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
//	if (HAL_I2C_Init(i2c_handle) != HAL_OK)
//	{
//		_Error_Handler(__FILE__, __LINE__);
//	}
//
//	return HAL_OK;
//}

HAL_StatusTypeDef BH1750_send_command(BH1750_device_t* dev, uint8_t cmd)
{
	//TODO hal checks
	if(HAL_I2C_Master_Transmit(
			dev->i2c_handle,	//I2C Handle
			dev->address_w,		//I2C addr of dev
			&cmd,				//CMD to be executed
			1,					//8bit addr
			10					//Wait time
		) != HAL_OK) return HAL_ERROR;

	return HAL_OK;
}

void BH1750_poll_self(BH1750_device_t* self, DeviceData *deviceData)
{
	BH1750_get_lumen(self, deviceData);
}

BH1750_device_t* BH1750_init_dev_struct(I2C_HandleTypeDef* i2c_handle,
		char* name, bool addr_grounded)
{
	BH1750_device_t* init =
			(BH1750_device_t*)calloc(1, sizeof(BH1750_device_t));

	if(init == NULL) return NULL;

	if(addr_grounded){
		init->address_r = BH1750_GROUND_ADDR_READ;
		init->address_w = BH1750_GROUND_ADDR_WRITE;
	}else{
		init->address_r = BH1750_NO_GROUND_ADDR_READ;
		init->address_w = BH1750_NO_GROUND_ADDR_WRITE;
	}

	init->name = (char*)malloc(sizeof(char) * strlen(name));

	if(init->name == NULL) return NULL;

	init->i2c_handle = i2c_handle;

	strcpy(init->name, name);

	init->poll = &BH1750_poll_self;

	return init;
}

HAL_StatusTypeDef BH1750_init_dev(BH1750_device_t* dev)
{
	BH1750_send_command(dev, CMD_POWER_ON);
	BH1750_send_command(dev, CMD_RESET);
	BH1750_send_command(dev, CMD_H_RES_MODE);

	return HAL_OK;
}

HAL_StatusTypeDef BH1750_read_dev(BH1750_device_t* dev)
{
	if(HAL_I2C_Master_Receive(dev->i2c_handle,
			dev->address_r,
			dev->buffer,
			2,
			10
	) != HAL_OK) return HAL_ERROR;

	return HAL_OK;
}

uint16_t BH1750_convert(BH1750_device_t* dev)
{
	dev->value = dev->buffer[0];
	dev->value = (dev->value << 8) | dev->buffer[1];

	return dev->value/=1.2;
}

HAL_StatusTypeDef BH1750_get_lumen(BH1750_device_t* dev, DeviceData *deviceData)
{
	BH1750_read_dev(dev);

	deviceData->BH1750_Lumens = BH1750_convert(dev);
	return HAL_OK;
}
