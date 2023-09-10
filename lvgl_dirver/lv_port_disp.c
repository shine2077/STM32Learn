//
// Created by sh956 on 2023/9/3.
//
#include "lv_port_disp.h"

static lv_disp_drv_t disp_drv;

static void disp_flush(lv_disp_drv_t * disp, const lv_area_t * area, lv_color_t * color_p)
{

    uint32_t OffLineSrc = 1024 - (area->x2 - area->x1 +1);
    void* pDist = &(((uint16_t*)ATK_RGBLCD_LTDC_LAYER_FB_ADDR)[1024*area->y1 + area->x1]);

    // 模式
    __HAL_RCC_DMA2D_CLK_ENABLE();

    DMA2D->CR = 0x00000000UL;
    // 源地址
    DMA2D->FGMAR   = (uint32_t)(uint16_t*)(color_p);
    // 目标地址
    DMA2D->OMAR    = (uint32_t)pDist;
    // 输入偏移
    DMA2D->FGOR    = 0;
    // 输出偏移
    DMA2D->OOR     = OffLineSrc;
    /* 前景层和输出区域都采用的RGB565颜色格式 */
    DMA2D->FGPFCCR = DMA2D_OUTPUT_RGB565;
    // 多少行
    DMA2D->NLR = (area->y2-area->y1+1) | ((area->x2 -area->x1 +1) << 16);
    /* 启动传输 */
    DMA2D->CR |= DMA2D_CR_START;
    /*等待传输完成*/
    while (DMA2D->CR & DMA2D_CR_START) {}
    lv_disp_flush_ready(disp);
}

//lvgl显示接口初始化
void lv_port_disp_init(void)
{
    static lv_color_t color_buf1[1024*20];
    static lv_color_t color_buf2[1024*20];
    static lv_disp_draw_buf_t buf;
    lv_disp_draw_buf_init(&buf, color_buf1, color_buf2, 1024*20);


    lv_disp_drv_init(&disp_drv);

    disp_drv.draw_buf = &buf;
    disp_drv.flush_cb = disp_flush;
    disp_drv.hor_res = 1024;
    disp_drv.ver_res = 600;
    lv_disp_drv_register(&disp_drv);
}


