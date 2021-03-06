/*
 * LED.h
 *
 *  Created on: Oct 3, 2021
 *      Author: otimofieiev
 */

#ifndef INC_LED_H_
#define INC_LED_H_

#include "stm32f4xx_hal.h"

//Data:
//PB12 - 8 - D7
//PA11 - 7 up- D6
//PA12 - 6 up - D5
//PC5 - 3 up - D4

#define D4_PORT GPIOC
#define D4_PIN GPIO_PIN_5
#define D5_PORT GPIOA
#define D5_PIN GPIO_PIN_12
#define D6_PORT GPIOA
#define D6_PIN GPIO_PIN_11
#define D7_PORT GPIOB
#define D7_PIN GPIO_PIN_12

//Chip managment:
//PC8 - RW - 1 up
//PC6 - EN - 2 up
//PB13 - RS - 5 down

#define RS_PORT GPIOB
#define RS_PIN GPIO_PIN_13
#define RW_PORT GPIOC
#define RW_PIN GPIO_PIN_8
#define EN_PORT GPIOC
#define EN_PIN GPIO_PIN_6

//-----[ Prototypes For All Functions ]-----

void LCD_Init();                  // Initialize The LCD For 4-Bit Interface
void LCD_Clear();                 // Clear The LCD Display
void LCD_SL();                    // Shift The Entire Display To The Left
void LCD_SR();                    // Shift The Entire Display To The Right
void LCD_CMD(unsigned char);      // Send Command To LCD
void LCD_DATA(unsigned char);     // Send 4-Bit Data To LCD
void LCD_Set_Cursor(unsigned char, unsigned char);  // Set Cursor Position
void LCD_Write_Char(char);        // Write Character To LCD At Current Position
void LCD_Write_String(char*);     // Write A String To LCD

#endif /* INC_LED_H_ */
