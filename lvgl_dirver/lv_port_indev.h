//
// Created by sh956 on 2023/9/3.
//

#ifndef _LV_PORT_INDEV_H
#define _LV_PORT_INDEV_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

void lv_port_indev_init(void);

void touchpad_read(lv_indev_drv_t * indev_drv, lv_indev_data_t * data);

#ifdef __cplusplus
}
#endif

#endif //_LV_PORT_INDEV_H
