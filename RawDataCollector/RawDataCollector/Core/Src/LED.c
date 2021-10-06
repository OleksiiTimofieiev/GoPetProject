/*
 * LED.c
 *
 *  Created on: Oct 3, 2021
 *      Author: otimofieiev
 */

#include "LED.h"

void LCD_Set_Cursor(unsigned char r, unsigned char c)
{
    unsigned char Temp,Low4,High4;
    if(r == 1)
    {
      Temp  = 0x80 + c - 1; //0x80 is used to move the cursor
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD_CMD(High4);
      LCD_CMD(Low4);
    }
    if(r == 2)
    {
      Temp  = 0xC0 + c - 1;
      High4 = Temp >> 4;
      Low4  = Temp & 0x0F;
      LCD_CMD(High4);
      LCD_CMD(Low4);
    }
    HAL_Delay(2);
}
void LCD_Clear()
{
    LCD_CMD(0);
    LCD_CMD(1);
    HAL_Delay(70);
}
void LCD_DATA(unsigned char Data)
{
    if(Data & 1)
    	HAL_GPIO_WritePin(D4_PORT, D4_PIN, 1);
    else
    	HAL_GPIO_WritePin(D4_PORT, D4_PIN, 0);
    if(Data & 2)
    	HAL_GPIO_WritePin(D5_PORT, D5_PIN, 1);
    else
    	HAL_GPIO_WritePin(D5_PORT, D5_PIN, 0);
    if(Data & 4)
    	HAL_GPIO_WritePin(D6_PORT, D6_PIN, 1);
    else
    	HAL_GPIO_WritePin(D6_PORT, D6_PIN, 0);
    if(Data & 8)
    	HAL_GPIO_WritePin(D7_PORT, D7_PIN, 1);
    else
    	HAL_GPIO_WritePin(D7_PORT, D7_PIN, 0);
}
void LCD_CMD(unsigned char a_CMD)
{
    // Select Command Register
    HAL_GPIO_WritePin(RS_PORT, RS_PIN, 0);
    // Move The Command Data To LCD
    LCD_DATA(a_CMD);
    // Send The EN Clock Signal
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, 1);
    HAL_Delay(50);
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, 0);
    HAL_Delay(50);
}
void LCD_Init(){
	LCD_DATA(0x00);
	    HAL_Delay(150);
	    LCD_CMD(0x03);
	    HAL_Delay(5);
	    LCD_CMD(0x03);
	    HAL_Delay(150);
	    LCD_CMD(0x03);
	    LCD_CMD(0x02);
	    LCD_CMD(0x02);
	    LCD_CMD(0x08);
	    LCD_CMD(0x00);
	    LCD_CMD(0x0C);
	    LCD_CMD(0x00);
	    LCD_CMD(0x06);
	    LCD_CMD(0x00);
	    LCD_CMD(0x01);
}

void LCD_Write_Char(char Data)
{
   char Low4,High4;
   Low4  = Data & 0x0F;
   High4 = Data & 0xF0;

   HAL_GPIO_WritePin(RS_PORT, RS_PIN, 1);

   LCD_DATA(High4>>4);
   HAL_GPIO_WritePin(EN_PORT, EN_PIN, 1);
   HAL_Delay(10);
   HAL_GPIO_WritePin(EN_PORT, EN_PIN, 0);
   HAL_Delay(10);

   LCD_DATA(Low4);
   HAL_GPIO_WritePin(EN_PORT, EN_PIN, 1);
   HAL_Delay(10);
   HAL_GPIO_WritePin(EN_PORT, EN_PIN, 0);
   HAL_Delay(10);
}
void LCD_Write_String(char *str)
{
    int i;
    for(i=0;str[i]!='\0';i++)
       LCD_Write_Char(str[i]);
}

void LCD_SL()
{
    LCD_CMD(0x01);
    LCD_CMD(0x08);
    HAL_Delay(35);
}
void LCD_SR()
{
    LCD_CMD(0x01);
    LCD_CMD(0x0C);
    HAL_Delay(35);
}

