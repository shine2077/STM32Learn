//
// Created by sh956 on 2023/8/30.
//

#ifndef _FT5426_H
#define _FT5426_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "ltdc.h"

#include <stdint.h>

//与电容触摸屏连接的芯片引脚(未包含IIC引脚) 
//IO操作函数	  
#define FT_RST  PIout(8)//FT5426复位引脚
#define FT_INT  PHin(7) //FT5426断引脚

//I2C读写命令	
#define FT_CMD_WR 				0X70    	//写命令
#define FT_CMD_RD 				0X71		//读命令

//FT5206 部分寄存器定义 
#define FT_DEVIDE_MODE 			0x00   		//FT5426模式控制寄存器
#define FT_REG_NUM_FINGER       0x02		//触摸状态寄存器

#define FT_TP1_REG 				0X03	  	//第一个触摸点数据地址
#define FT_TP2_REG 				0X09		//第二个触摸点数据地址
#define FT_TP3_REG 				0X0F		//第三个触摸点数据地址
#define FT_TP4_REG 				0X15		//第四个触摸点数据地址
#define FT_TP5_REG 				0X1B		//第五个触摸点数据地址  


#define	FT_ID_G_LIB_VERSION		0xA1		//版本		
#define FT_ID_G_MODE 			0xA4   		//FT5426中断模式控制寄存器
#define FT_ID_G_THGROUP			0x80   		//触摸有效值设置寄存器
#define FT_ID_G_PERIODACTIVE	0x88   		//激活状态周期设置寄存器


uint8_t FT5426_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void FT5426_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
uint8_t FT5426_Init(void);
uint8_t FT5426_Scan(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif //_FT5426_H
