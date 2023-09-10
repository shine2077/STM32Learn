//
// Created by sh956 on 2023/9/10.
//

#ifndef GUI_DEMO_DRAW_H
#define GUI_DEMO_DRAW_H

#include "main.h"

//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
#define LIGHTBLUE      	 0X7D7C

void DrawPad();
void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
void gui_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color);
void gui_fill_circle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color);
void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t size,uint16_t color);



#endif //GUI_DEMO_DRAW_H
