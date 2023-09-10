//
// Created by sh956 on 2023/9/3.
//

#include "iic.h"

static void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told,tnow,tcnt=0;
    uint32_t reload=SysTick->LOAD;				//LOAD的值
    ticks=nus*180; 						//需要的节拍数
    told=SysTick->VAL;        				//刚进入时的计数器值
    while(1)
    {
        tnow=SysTick->VAL;
        if(tnow!=told)
        {
            if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
            else tcnt+=reload-tnow+told;
            told=tnow;
            if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
        }
    };
}

void CT_Delay(void)
{
    delay_us(2);
}

void CT_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;

    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOI_CLK_ENABLE();

    GPIO_Initure.Pin = GPIO_PIN_6;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);

    GPIO_Initure.Pin = GPIO_PIN_3;              //PI3
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);
    CT_IIC_SDA = 1;
    CT_IIC_SCL = 1;
}

void CT_IIC_Start(void)
{
    CT_IIC_SDA = 1;
    CT_IIC_SCL = 1;
    CT_Delay();
    CT_IIC_SDA = 0;
    CT_Delay();
    CT_IIC_SCL = 0;
    CT_Delay();
}

void CT_IIC_Stop(void)
{
    CT_IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    CT_Delay();
    CT_IIC_SCL = 1;
    CT_Delay();
    CT_IIC_SDA = 1;
    CT_Delay();
}

uint8_t CT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    uint8_t rack = 0;

    CT_IIC_SDA = 1;
    CT_Delay();
    CT_IIC_SCL = 1;
    CT_Delay();

    while (CT_READ_SDA)
    {
        ucErrTime++;

        if (ucErrTime > 250)
        {
            CT_IIC_Stop();
            rack = 1;
            break;
        }

        CT_Delay();
    }

    CT_IIC_SCL = 0;
    CT_Delay();

    return rack;
}

void CT_IIC_Ack(void)
{
    CT_IIC_SDA = 0;
    CT_Delay();
    CT_IIC_SCL = 1;
    CT_Delay();
    CT_IIC_SCL = 0;
    CT_Delay();
    CT_IIC_SDA = 1;
    CT_Delay();
}

void CT_IIC_NAck(void)
{
    CT_IIC_SDA = 1;
    CT_Delay();
    CT_IIC_SCL = 1;
    CT_Delay();
    CT_IIC_SCL = 0;
    CT_Delay();
}

void CT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;

    for (t = 0; t < 8; t++)
    {
        CT_IIC_SDA = (txd & 0x80) >> 7;
        CT_Delay();
        CT_IIC_SCL = 1;
        CT_Delay();
        CT_IIC_SCL = 0;
        txd <<= 1;
    }

    CT_IIC_SDA = 1;
}

uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;

    for (i = 0; i < 8; i++ )
    {
        receive <<= 1;
        CT_IIC_SCL = 1;
        CT_Delay();

        if (CT_READ_SDA)receive++;

        CT_IIC_SCL = 0;
        CT_Delay();
    }

    if (!ack)CT_IIC_NAck();
    else CT_IIC_Ack();

    return receive;
}