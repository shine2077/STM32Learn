//
// Created by sh956 on 2023/9/3.
//

#ifndef __IIC_H__
#define __IIC_H__

#include "main.h"

#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010


#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr))
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)
#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)

//IO方向设置
#define CT_SDA_IN()  {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=0<<3*2;}
#define CT_SDA_OUT() {GPIOI->MODER&=~(3<<(3*2));GPIOI->MODER|=1<<3*2;}
//IO操作函数
#define CT_IIC_SCL  PHout(6)  //SCL
#define CT_IIC_SDA  PIout(3)  //SDA
#define CT_READ_SDA PIin(3)   //输入SDA


//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口
void CT_IIC_Start(void);				//发送IIC开始信号
void CT_IIC_Stop(void);	  				//发送IIC停止信号
void CT_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t CT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CT_IIC_Ack(void);					//IIC发送ACK信号
void CT_IIC_NAck(void);					//IIC不发送ACK信号

#endif //__IIC_H__
