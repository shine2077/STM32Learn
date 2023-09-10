//
// Created by sh956 on 2023/8/30.
//

#ifndef _TOUCHPAD_H
#define _TOUCHPAD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

#include "string.h"

#define TP_PRES_DOWN 0x80  //触屏被按下
#define TP_CATH_PRES 0x40  //有按键按下了
#define CT_MAX_TOUCH 10

//触摸屏控制器
typedef struct
{
    uint8_t (*init)(void);			//初始化触摸屏控制器
    uint8_t (*scan)(uint8_t);				//扫描触摸屏.0,屏幕扫描;1,物理坐标;
    uint16_t x[CT_MAX_TOUCH]; 		//当前坐标
    uint16_t y[CT_MAX_TOUCH];		//电容屏有最多5组坐标,电阻屏则用x[0],y[0]代表:此次扫描时,触屏的坐标,用
    //x[9],y[9]存储第一次按下时的坐标. 
    uint16_t sta;					//笔的状态
    uint8_t touchtype;
}_m_tp_dev;

typedef struct
{
    uint16_t width;			//LCD 宽度
    uint16_t height;			//LCD 高度
    uint16_t id;				//LCD ID
    uint8_t  dir;			//横屏还是竖屏控制：0，竖屏；1，横屏。
}_lcd_dev;

//LCD参数
extern _lcd_dev lcddev;	//管理LCD重要参数

extern _m_tp_dev tp_dev;	 	//触屏控制器在touch.c里面定义

uint8_t TP_Init(void);								//初始化

#ifdef __cplusplus
}
#endif

#endif //DEMO_TOUCHPAD_H
