/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "HTU21.h"
#include "BH1750.h"
#include "BMP280.h"
#include "LED.h"
#include "EEPROM.h"

#include <stdio.h>
#include <string.h>
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
I2C_HandleTypeDef hi2c3;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2C3_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool checkData(DeviceData *deviceData, DeviceData *deviceDataCached);
void displayOnLCD(DeviceData *deviceData);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	BMP280_HandleTypedef bmp280;

	uint16_t size;
	uint8_t Data[256];
	DeviceData deviceData;
	DeviceData deviceDataCached;
	memset(&deviceData, 0, sizeof (deviceData));
	memset(&deviceDataCached, 0, sizeof (deviceDataCached));


	//// EEPROM block

//#define DEV_ADDR 0xa0
//uint8_t dataw1[] = "hello world from EEPROM";
//uint8_t dataw2[] = "This is the second string from EEPROM";
//float dataw3 = 1234.5678;
//
//uint8_t datar1[100];
//uint8_t datar2[100];
//float datar3;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_Delay(100);
//  for (int i=0; i<512; i++)
//  {
//	  EEPROM_PageErase(i);
//  }
//  EEPROM_Write(3, 0, dataw1, strlen((char *)dataw1));
//
//  EEPROM_Write(5, 20, dataw2, strlen((char *)dataw2));
//
//  EEPROM_Write_NUM (6, 0, dataw3);
//
//  EEPROM_Read(3, 0, datar1, 50);
//
//  EEPROM_Read(5, 15, datar2, 50);
//
//  datar3 = EEPROM_Read_NUM (6, 0);

  LCD_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  BH1750_device_t* lumen = BH1750_init_dev_struct(&hi2c3, "test device", true);
  BH1750_init_dev(lumen);

  bmp280_init_default_params(&bmp280.params);
  bmp280.addr = BMP280_I2C_ADDRESS_0;
  bmp280.i2c = &hi2c3;

  while (!bmp280_init(&bmp280, &bmp280.params))
  {
  	size = sprintf((char *)Data, "BMP280 initialization failed\n");
	HAL_Delay(100);
  }
  HAL_Delay(100);
//  HAL_UART_Receive_IT(&huart3, str, 1);
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */


		lumen->poll(lumen, &deviceData);
		HAL_Delay(100);

		readTempAndHumidity(&hi2c1, &deviceData);
		HAL_Delay(100);

		bmp280_read_float(&bmp280, &deviceData);

		HAL_Delay(100);
		if (measureData && deviceData.BH1750_Lumens != 0)
		{
			if (checkData(&deviceData, &deviceDataCached))
			{
				size = sprintf((char *)Data,"Lumen: %d lm; Temperature: %.d C; Humidity: %d %%; Pressure: %.d hPA\r\n",
						(int)deviceData.BH1750_Lumens,
						(int)deviceData.HTU21D_Temperature,
						(int)deviceData.HTU21D_Humidity,
						(int)deviceData.BMP280_Pressure);

				deviceDataCached.BH1750_Lumens = deviceData.BH1750_Lumens;
				deviceDataCached.BMP280_Pressure = deviceData.BMP280_Pressure;
				deviceDataCached.HTU21D_Humidity = deviceData.HTU21D_Humidity;
				deviceDataCached.HTU21D_Temperature = deviceData.HTU21D_Temperature;

				displayOnLCD(&deviceData);
				size = sprintf((char *)Data,"%d;%d;%d;%d",
										(int)deviceData.BH1750_Lumens,
										(int)deviceData.HTU21D_Temperature,
										(int)deviceData.HTU21D_Humidity,
										(int)deviceData.BMP280_Pressure);
				HAL_UART_Transmit(&huart3, Data, size, 5000);
			}

		}
		memset(Data, 0, sizeof(Data));
		HAL_Delay(5000);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2C3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LD2_Pin|GPIO_PIN_11|GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12|GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LD2_Pin PA11 PA12 */
  GPIO_InitStruct.Pin = LD2_Pin|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC5 PC6 PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB12 PB13 */
  GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB14 */
  GPIO_InitStruct.Pin = GPIO_PIN_14;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
bool checkData(DeviceData *deviceData, DeviceData *deviceDataCached)
{
	if (deviceData->BH1750_Lumens != deviceDataCached->BH1750_Lumens)
		return true;
	else if (deviceData->BMP280_Pressure != deviceDataCached->BMP280_Pressure)
		return true;
	else if (deviceData->HTU21D_Humidity != deviceDataCached->HTU21D_Humidity)
		return true;
	else if (deviceData->HTU21D_Temperature != deviceDataCached->HTU21D_Temperature)
		return true;

	return false;
}

void displayOnLCD(DeviceData *deviceData)
{
	// L: lm
	// T: C
	// H: %
	// P: hPa
	char LCD_Data_Lumen[3] = "";
	char LCD_Data_Lumen_2[8]="";

	char LCD_Data_Temperature[2] = "";
	char LCD_Data_Temperature_2[8]="";

	char LCD_Data_Humidity[2] = "";
	char LCD_Data_Humidity_2[8]="";

	char LCD_Data_Pressure[5] = "";
	char LCD_Data_Pressure_2[10]="";

	itoa(deviceData->BH1750_Lumens, LCD_Data_Lumen, 10);
	sprintf(LCD_Data_Lumen_2, "L:%s", LCD_Data_Lumen);

	itoa((int)deviceData->HTU21D_Temperature,LCD_Data_Temperature, 10);
	sprintf(LCD_Data_Temperature_2, "T:%sC", LCD_Data_Temperature);

	itoa((int)deviceData->HTU21D_Humidity,LCD_Data_Humidity, 10);
	sprintf(LCD_Data_Humidity_2, "H:%s%%", LCD_Data_Humidity);

	itoa((int)deviceData->BMP280_Pressure,LCD_Data_Pressure, 10);
	sprintf(LCD_Data_Pressure_2, "P:%shPa", LCD_Data_Pressure);

	LCD_Clear();

	LCD_Set_Cursor(1,1);
	LCD_Write_String(LCD_Data_Lumen_2);

	LCD_Set_Cursor(1,8);
	LCD_Write_String(LCD_Data_Temperature_2);

	LCD_Set_Cursor(2,1);
	LCD_Write_String(LCD_Data_Humidity_2);

	LCD_Set_Cursor(2,8);
	LCD_Write_String(LCD_Data_Pressure_2);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
