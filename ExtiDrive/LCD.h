/******************************************************************************/
/* LCD.h: Graphic LCD function prototypes and defines                        */
/******************************************************************************/
/* This file is part of the uVision/ARM development tools.                    */
/* Copyright (c) 2005-2009 Keil Software. All rights reserved.                */
/* This software may only be used under the terms of a valid, current,        */
/* end user licence from KEIL for a compatible version of KEIL software       */
/* development tools. Nothing else gives you the right to use this software.  */
/******************************************************************************/

#ifndef _LCD_H
#define _LCD_H

/*------------------------------------------------------------------------------
  Color coding
  LCD is coded:   15..11 red, 10..5 green, 4..0 blue  (unsigned short)  LCD_R5, LCD_G6, LCD_B5   
  original coding: 17..12 red, 11..6 green, 5..0 blue                    ORG_R6,  ORG_G6,  ORG_B6

  ORG_R1..5 = LCD_R0..4,  ORG_R0 = LCD_R4
  ORG_G0..5 = LCD_G0..5,
  ORG_B1..5 = LCD_B0..4,  ORG_B0 = LCD_B4
 *----------------------------------------------------------------------------*/
                            
/* LCD RGB color definitions                                                 */
#define Black           0x0000		  /*   0,   0,   0 */
#define Navy            0x000F      /*   0,   0, 128 */
#define DarkGreen       0x03E0      /*   0, 128,   0 */
#define DarkCyan        0x03EF      /*   0, 128, 128 */
#define Maroon          0x7800      /* 128,   0,   0 */
#define Purple          0x780F      /* 128,   0, 128 */
#define Olive           0x7BE0      /* 128, 128,   0 */
#define LightGrey       0xC618      /* 192, 192, 192 */
#define DarkGrey        0x7BEF      /* 128, 128, 128 */
#define Blue            0x001F      /*   0,   0, 255 */
#define Green           0x07E0      /*   0, 255,   0 */
#define Cyan            0x07FF      /*   0, 255, 255 */
#define Red             0xF800      /* 255,   0,   0 */
#define Magenta         0xF81F      /* 255,   0, 255 */
#define Yellow          0xFFE0      /* 255, 255, 0   */
#define White           0xFFFF      /* 255, 255, 255 */


void LCD_Init           (void);
void LCD_WindowMax (unsigned int x,unsigned int y,unsigned int x_end,unsigned int y_end);
void LCD_PutPixel       (unsigned int x, unsigned int y);
void LCD_SetTextColor   (unsigned short color);
void LCD_SetBackColor   (unsigned short color);
void LCD_Clear          (unsigned short color);
void LCD_DrawChar (unsigned int x, unsigned int y, unsigned char c);
void LCD_DisplayNum(unsigned int x,unsigned int y,unsigned int num,unsigned int len);
void LCD_DisplayChar    (unsigned int ln, unsigned int col, unsigned char  c);
void LCD_DisplayString  (unsigned int ln, unsigned int col, unsigned char *s);
void LCD_ClearLn        (unsigned int ln);
void LCD_Bargraph       (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int val);
void LCD_Bitmap         (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bitmap);
void LCD_Bmp            (unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char *bmp);

#endif /* _LCD_H */
