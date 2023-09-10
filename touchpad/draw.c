//
// Created by sh956 on 2023/9/10.
//
#include "draw.h"
#include "touchpad.h"

void LTDC_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color)
{
    uint32_t psx,psy,pex,pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
    uint16_t offline;

    psx=sx;psy=sy;
    pex=ex;pey=ey;

    offline=1024-(pex-psx+1);
    void* pDist = &(((uint16_t*)ATK_RGBLCD_LTDC_LAYER_FB_ADDR)[psy*1024 + psx]);

    DMA2D->CR=0x00030000UL;
    DMA2D->OPFCCR=LTDC_PIXEL_FORMAT_RGB565;	//设置颜色格式
    DMA2D->OOR=offline;				//设置行偏移
    DMA2D->OMAR=(uint32_t)pDist;				//输出存储器地址
    DMA2D->NLR=(pey-psy+1)|((pex-psx+1)<<16);	//设定行数寄存器
    DMA2D->OCOLR=color;						//设定输出颜色寄存器

    DMA2D->CR|=DMA2D_CR_START;				//启动DMA2D

    while(DMA2D->CR & DMA2D_CR_START){}
}

void gui_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
    if(len==0)return;
    if((x0+len-1)>=lcddev.width)x0=lcddev.width-len-1;	//限制坐标范围
    if(y0>=lcddev.height)y0=lcddev.height-1;			//限制坐标范围
    LTDC_Fill(x0,y0,x0+len-1,y0,color);
}

void gui_fill_circle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color)
{
    uint32_t i;
    uint32_t imax = ((uint32_t)r*707)/1000+1;
    uint32_t sqmax = (uint32_t)r*(uint32_t)r+(uint32_t)r/2;
    uint32_t x=r;
    gui_draw_hline(x0-r,y0,2*r,color);
    for (i=1;i<=imax;i++)
    {
        if ((i*i+x*x)>sqmax)// draw lines from outside
        {
            if (x>imax)
            {
                gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
                gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
            }
            x--;
        }
        // draw lines from inside (center)
        gui_draw_hline(x0-x,y0+i,2*x,color);
        gui_draw_hline(x0-x,y0-i,2*x,color);
    }
}

void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t size,uint16_t color)
{
    uint16_t t;
    int xerr=0,yerr=0,delta_x,delta_y,distance;
    int incx,incy,uRow,uCol;
    if(x1<size|| x2<size||y1<size|| y2<size)return;
    delta_x=x2-x1; //计算坐标增量
    delta_y=y2-y1;
    uRow=x1;
    uCol=y1;
    if(delta_x>0)incx=1; //设置单步方向
    else if(delta_x==0)incx=0;//垂直线
    else {incx=-1;delta_x=-delta_x;}
    if(delta_y>0)incy=1;
    else if(delta_y==0)incy=0;//水平线
    else{incy=-1;delta_y=-delta_y;}
    if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴
    else distance=delta_y;
    for(t=0;t<=distance+1;t++ )//画线输出
    {
        gui_fill_circle(uRow,uCol,size,color);//画点
        xerr+=delta_x ;
        yerr+=delta_y ;
        if(xerr>distance)
        {
            xerr-=distance;
            uRow+=incx;
        }
        if(yerr>distance)
        {
            yerr-=distance;
            uCol+=incy;
        }
    }
}
