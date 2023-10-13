#ifndef __OLED_H
#define __OLED_H

#include "stm32f4xx_hal.h"
#include "main.h"

//-----------------OLED端口定义----------------

#define OLED_SCL_Clr() HAL_GPIO_WritePin(SPI_SCK_GPIO_Port, SPI_SCK_Pin, GPIO_PIN_RESET) // SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(SPI_SCK_GPIO_Port, SPI_SCK_Pin, GPIO_PIN_SET)

#define OLED_SDA_Clr() HAL_GPIO_WritePin(SPI_SDA_GPIO_Port, SPI_SDA_Pin, GPIO_PIN_RESET) // SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(SPI_SDA_GPIO_Port, SPI_SDA_Pin, GPIO_PIN_SET)

#define OLED_RES_Clr() HAL_GPIO_WritePin(SPI_Screen_RES_GPIO_Port, SPI_Screen_RES_Pin, GPIO_PIN_RESET) // RES
#define OLED_RES_Set() HAL_GPIO_WritePin(SPI_Screen_RES_GPIO_Port, SPI_Screen_RES_Pin, GPIO_PIN_SET)

#define OLED_DC_Clr() HAL_GPIO_WritePin(SPI_Screen_DC_GPIO_Port, SPI_Screen_DC_Pin, GPIO_PIN_RESET) // DC
#define OLED_DC_Set() HAL_GPIO_WritePin(SPI_Screen_DC_GPIO_Port, SPI_Screen_DC_Pin, GPIO_PIN_SET)

#define OLED_CS_Clr() HAL_GPIO_WritePin(SPI_Screen_CS_GPIO_Port, SPI_Screen_CS_Pin, GPIO_PIN_RESET) // CS
#define OLED_CS_Set() HAL_GPIO_WritePin(SPI_Screen_CS_GPIO_Port, SPI_Screen_CS_Pin, GPIO_PIN_SET)

#define OLED_CMD 0  // 写命令
#define OLED_DATA 1 // 写数据

#define u8 unsigned char
#define u16 unsigned int
#define u32 unsigned long

void OLED_ClearPoint(u8 x, u8 y);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat, u8 mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_DrawLine(u8 x1, u8 y1, u8 x2, u8 y2, u8 mode);
void OLED_DrawCircle(u8 x, u8 y, u8 r);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size1, u8 mode);
void OLED_ShowChar6x8(u8 x, u8 y, u8 chr, u8 mode);
void OLED_ShowString(u8 x, u8 y, u8 *chr, u8 size1, u8 mode);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, u8 size1, u8 mode);
void OLED_ShowChinese(u8 x, u8 y, u8 num, u8 size1, u8 mode);
void OLED_ScrollDisplay(u8 num, u8 space, u8 mode);
void OLED_ShowPicture(u8 x, u8 y, u8 sizex, u8 sizey, u8 BMP[], u8 mode);
void OLED_Init(void);

#endif
