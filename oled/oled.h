#ifndef __OLED_H
#define __OLED_H
#include <msp430.h>
#include "parameter_type.h"
#define OLED_MODE 0
#define SIZE 8
#define XLevelL     0x00
#define XLevelH     0x10
#define Max_Column  128
#define Max_Row     64
#define Brightness  0xFF
#define X_WIDTH     128
#define Y_WIDTH     64

#define OLED_SCLK_Clr() P6OUT&=~BIT3  //CLK
#define OLED_SCLK_Set() P6OUT|=BIT3

#define OLED_SDIN_Clr() P6OUT&=~BIT4  //DIN
#define OLED_SDIN_Set() P6OUT|=BIT4

#define OLED_CMD  0
#define OLED_DATA 1


// Functions to control OLED are as follows
void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8 x,uint8 y,uint8 t);
void OLED_Fill(uint8 x1,uint8 y1,uint8 x2,uint8 y2,uint8 dot);
void OLED_ShowChar(uint8 x,uint8 y,char chr,uint8 Char_Size);
void OLED_ShowNum(uint8 x,uint8 y,uint32 num,uint8 len,uint8 size);
void OLED_ShowVI(uint8 x,uint8 y,uint32 num,uint8 size);
void OLED_ShowFloat(uint8 x, uint8 y, float num, uint8 size);
void OLED_ShowString(uint8 x,uint8 y, char *p,uint8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture();
void IIC_Start();
void IIC_Stop();
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack();
void OLED_DrawBMP(uint8 x, uint8 y, uint8 sizex, uint8 sizey, uint8 BMP[]);
void OLED_Prepare();
#endif

