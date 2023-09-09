/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : FMC.c
  * Description        : This file provides code for the configuration
  *                      of the FMC peripheral.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "fmc.h"

/* USER CODE BEGIN 0 */
void sdram_initialization_sequence(void)
{
    uint32_t temp = 0;
    /* SDRAM 控制器初始化完成以后还需要按照如下顺序初始化 SDRAM */
    sdram_send_cmd(FMC_SDRAM_CMD_CLK_ENABLE, 1, 0); /* 时钟配置使能 */
    HAL_Delay(500);                                   /* 至少延时 200us */
    sdram_send_cmd(FMC_SDRAM_CMD_PALL, 1, 0);       /* 对所有存储区预充电 */
    sdram_send_cmd(FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);  /* 设置自刷新次数 */
    /* 配置模式寄存器,SDRAM 的 bit0~bit2 为指定突发访问的长度，
     * bit3 为指定突发访问的类型，bit4~bit6 为 CAS 值，bit7 和 bit8 为运行模式
     * bit9 为指定的写突发模式，bit10 和 bit11 位保留位 */
    temp = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1|SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL|SDRAM_MODEREG_CAS_LATENCY_3|
            SDRAM_MODEREG_OPERATING_MODE_STANDARD|SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    sdram_send_cmd(FMC_SDRAM_CMD_LOAD_MODE,1,temp);/* 设置 SDRAM 的模式寄存器 */
}

uint8_t sdram_send_cmd(uint8_t cmd, uint8_t refresh, uint16_t regval)
{
    FMC_SDRAM_CommandTypeDef command;

    command.CommandMode = cmd;                /* 命令 */
    command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;      /* 目标 SDRAM 存储区域 */
    command.AutoRefreshNumber = refresh;      /* 自刷新次数 */
    command.ModeRegisterDefinition = regval;  /* 要写入模式寄存器的值 */

    if (HAL_SDRAM_SendCommand(&hsdram1, &command, 0X1000) == HAL_OK)
    {/* 向 SDRAM 发送命令 */
        return 0;
    }
    else return 1;
}
/* USER CODE END 0 */

SDRAM_HandleTypeDef hsdram1;

/* FMC initialization function */
void MX_FMC_Init(void)
{
  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 8;
  SdramTiming.SelfRefreshTime = 6;
  SdramTiming.RowCycleDelay = 6;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */
  sdram_initialization_sequence();/* 发送 SDRAM 初始化序列 */

  HAL_SDRAM_ProgramRefreshRate(&hsdram1, 683);
  /* USER CODE END FMC_Init 2 */
}

static uint32_t FMC_Initialized = 0;

static void HAL_FMC_MspInit(void){
  /* USER CODE BEGIN FMC_MspInit 0 */

  /* USER CODE END FMC_MspInit 0 */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if (FMC_Initialized) {
    return;
  }
  FMC_Initialized = 1;

  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_ENABLE();

  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PC0   ------> FMC_SDNWE
  PC2   ------> FMC_SDNE0
  PC3   ------> FMC_SDCKE0
  PF11   ------> FMC_SDNRAS
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PE11   ------> FMC_D8
  PE12   ------> FMC_D9
  PE13   ------> FMC_D10
  PE14   ------> FMC_D11
  PE15   ------> FMC_D12
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PG2   ------> FMC_A12
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15   ------> FMC_SDNCAS
  */
  /* GPIO_InitStruct */
  /* USER CODE BEGIN FMC_MspInit 1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;    /* 推挽复用 */
    GPIO_InitStruct.Pull = GPIO_PULLUP;        /* 上拉 */
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;   /* 高速 */
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC; /* 复用为 FMC */
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);    /* 初始化 PC0,2,3 */

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                          GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);     /* 初始化 PD0,1,8,9,10,14,15 */

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_7 | GPIO_PIN_8 |
                          GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                          GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    /* 初始化 PE0,1,7,8,9,10,11,12,13,14,15 */

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                          GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                          GPIO_PIN_14 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);
    /* 初始化 PF0,1,2,3,4,5,11,12,13,14,15 */

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_4 |
                          GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
    HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);     /* 初始化 PG0,1,2,4,5,8,15 */
  /* USER CODE END FMC_MspInit 1 */
}

void HAL_SDRAM_MspInit(SDRAM_HandleTypeDef* sdramHandle){
  /* USER CODE BEGIN SDRAM_MspInit 0 */

  /* USER CODE END SDRAM_MspInit 0 */
  HAL_FMC_MspInit();
  /* USER CODE BEGIN SDRAM_MspInit 1 */

  /* USER CODE END SDRAM_MspInit 1 */
}

static uint32_t FMC_DeInitialized = 0;

static void HAL_FMC_MspDeInit(void){
  /* USER CODE BEGIN FMC_MspDeInit 0 */

  /* USER CODE END FMC_MspDeInit 0 */
  if (FMC_DeInitialized) {
    return;
  }
  FMC_DeInitialized = 1;
  /* Peripheral clock enable */
  __HAL_RCC_FMC_CLK_DISABLE();

  /** FMC GPIO Configuration
  PF0   ------> FMC_A0
  PF1   ------> FMC_A1
  PF2   ------> FMC_A2
  PF3   ------> FMC_A3
  PF4   ------> FMC_A4
  PF5   ------> FMC_A5
  PC0   ------> FMC_SDNWE
  PC2   ------> FMC_SDNE0
  PC3   ------> FMC_SDCKE0
  PF11   ------> FMC_SDNRAS
  PF12   ------> FMC_A6
  PF13   ------> FMC_A7
  PF14   ------> FMC_A8
  PF15   ------> FMC_A9
  PG0   ------> FMC_A10
  PG1   ------> FMC_A11
  PE7   ------> FMC_D4
  PE8   ------> FMC_D5
  PE9   ------> FMC_D6
  PE10   ------> FMC_D7
  PE11   ------> FMC_D8
  PE12   ------> FMC_D9
  PE13   ------> FMC_D10
  PE14   ------> FMC_D11
  PE15   ------> FMC_D12
  PD8   ------> FMC_D13
  PD9   ------> FMC_D14
  PD10   ------> FMC_D15
  PD14   ------> FMC_D0
  PD15   ------> FMC_D1
  PG2   ------> FMC_A12
  PG4   ------> FMC_BA0
  PG5   ------> FMC_BA1
  PG8   ------> FMC_SDCLK
  PD0   ------> FMC_D2
  PD1   ------> FMC_D3
  PG15   ------> FMC_SDNCAS
  */

  HAL_GPIO_DeInit(GPIOF, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_2|GPIO_PIN_3);

  HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_8|GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15);

  HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_14
                          |GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1);

  /* USER CODE BEGIN FMC_MspDeInit 1 */

  /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SDRAM_MspDeInit(SDRAM_HandleTypeDef* sdramHandle){
  /* USER CODE BEGIN SDRAM_MspDeInit 0 */

  /* USER CODE END SDRAM_MspDeInit 0 */
  HAL_FMC_MspDeInit();
  /* USER CODE BEGIN SDRAM_MspDeInit 1 */

  /* USER CODE END SDRAM_MspDeInit 1 */
}
/**
  * @}
  */

/**
  * @}
  */
