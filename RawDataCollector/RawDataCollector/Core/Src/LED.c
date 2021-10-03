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
//
//void send_to_lcd (char data, int rs)
//{
//    HAL_GPIO_WritePin(RS_PORT, RS_PIN, rs);  // rs = 1 for data, rs=0 for command
//    /* write the data to the respective pin */
//    HAL_Delay(100);
//    HAL_GPIO_WritePin(D7_PORT, D7_PIN, ((data>>3)&0x01));
//    HAL_GPIO_WritePin(D6_PORT, D6_PIN, ((data>>2)&0x01));
//    HAL_GPIO_WritePin(D5_PORT, D5_PIN, ((data>>1)&0x01));
//    HAL_GPIO_WritePin(D4_PORT, D4_PIN, ((data>>0)&0x01));
//    /* Toggle EN PIN to send the data
//     * if the HCLK > 100 MHz, use the  20 us delay
//     * if the LCD still doesn't work, increase the delay to 50, 80 or 100..
//     */
//    HAL_Delay(50);
//    HAL_GPIO_WritePin(EN_PORT, EN_PIN, 1);
////  delay (20);
//    HAL_Delay(50);
//    HAL_GPIO_WritePin(EN_PORT, EN_PIN, 0);
////  delay (20);
//    HAL_Delay(50);
//}
//
//void lcd_send_cmd (char cmd)
//{
//    char datatosend;
//    /* send upper nibble first */
//    datatosend = ((cmd>>4)&0x0f);
//    send_to_lcd(datatosend,0);  // RS must be while sending command
//    /* send Lower Nibble */
//    datatosend = ((cmd)&0x0f);
//    send_to_lcd(datatosend, 0);
//}
//
//void lcd_send_data (char data)
//{
//    char datatosend;
//
//    /* send higher nibble */
//    datatosend = ((data>>4)&0x0f);
//    send_to_lcd(datatosend, 1);  // rs =1 for sending data
//    /* send Lower nibble */
//    datatosend = ((data)&0x0f);
//    send_to_lcd(datatosend, 1);
//}
//
//void lcd_put_cur(int row, int col)
//{
//    switch (row)
//    {
//        case 0:
//            col |= 0x80;
//            break;
//        case 1:
//            col |= 0xC0;
//            break;
//        default:
//        	break;
//    }
//    lcd_send_cmd(col);
//}
//
//void lcd_clear (void)
//{
//	lcd_send_cmd(0x01);
//	HAL_Delay(2);
//}
//
//void lcd_send_string (char *str)
//{
//	while (*str) lcd_send_data (*str++);
//}
//
//void lcd_init (void)
//{
//    // 4 bit initialisation
//    HAL_Delay(50);  // wait for >40ms
//    lcd_send_cmd (0x30);
//    HAL_Delay(5);  // wait for >4.1ms
//    lcd_send_cmd (0x30);
//    HAL_Delay(1);  // wait for >100us
//    lcd_send_cmd (0x30);
//    HAL_Delay(10);
//    lcd_send_cmd (0x20);  // 4bit mode
//    HAL_Delay(10);
//
//  // dislay initialisation
//    lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
//    HAL_Delay(1);
//    lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
//    HAL_Delay(1);
//    lcd_send_cmd (0x01);  // clear display
//    HAL_Delay(1);
//    HAL_Delay(1);
//    lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
//    HAL_Delay(1);
//    lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
//}

//const LCD16x2_CfgType LCD16x2_CfgParam =
//{
//	GPIOB,
//	GPIO_PIN_5,
//	GPIO_PIN_6,
//	GPIO_PIN_7,
//	GPIO_PIN_8,
//	GPIO_PIN_3,
//	GPIO_PIN_4,
//	20
//};
