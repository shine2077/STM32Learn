//
// Created by sh956 on 2023/9/9.
//

#include "touchpad.h"
#include "ft5426.h"
#include "gt9147.h"

_m_tp_dev tp_dev={TP_Init,NULL,
                  0,0,0,0};
_lcd_dev lcddev={1024, 600,0X7016,1,};

uint8_t TP_Init(void)
{
    FT5426_Init();
    tp_dev.scan=FT5426_Scan;
    tp_dev.touchtype|=0X80;
    tp_dev.touchtype|=lcddev.dir&0X01;
    return 0;
}


