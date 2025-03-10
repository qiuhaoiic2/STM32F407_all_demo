/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "main.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}


/**
 * HardFault 处理程序，允许在调试器中检查故障状态
 */
void HardFault_Handler_C(uint32_t *hardfault_args) {
    volatile uint32_t stacked_r0;
    volatile uint32_t stacked_r1;
    volatile uint32_t stacked_r2;
    volatile uint32_t stacked_r3;
    volatile uint32_t stacked_r12;
    volatile uint32_t stacked_lr;
    volatile uint32_t stacked_pc;
    volatile uint32_t stacked_psr;
    volatile uint32_t _CFSR;
    volatile uint32_t _HFSR;
    volatile uint32_t _DFSR;
    volatile uint32_t _AFSR;
    volatile uint32_t _BFAR;
    volatile uint32_t _MMAR;

    stacked_r0 = hardfault_args[0];
    stacked_r1 = hardfault_args[1];
    stacked_r2 = hardfault_args[2];
    stacked_r3 = hardfault_args[3];
    stacked_r12 = hardfault_args[4];
    stacked_lr = hardfault_args[5];
    stacked_pc = hardfault_args[6];
    stacked_psr = hardfault_args[7];

    // 获取故障状态寄存器的内容
    _CFSR = (*((volatile uint32_t *)(0xE000ED28)));
    _HFSR = (*((volatile uint32_t *)(0xE000ED2C)));
    _DFSR = (*((volatile uint32_t *)(0xE000ED30)));
    _AFSR = (*((volatile uint32_t *)(0xE000ED3C)));
    _BFAR = (*((volatile uint32_t *)(0xE000ED38)));
    _MMAR = (*((volatile uint32_t *)(0xE000ED34)));

    printf("\r\n== Hard Fault ==\r\n");
    printf("R0 = 0x%08lX\r\n", stacked_r0);
    printf("R1 = 0x%08lX\r\n", stacked_r1);
    printf("R2 = 0x%08lX\r\n", stacked_r2);
    printf("R3 = 0x%08lX\r\n", stacked_r3);
    printf("R12 = 0x%08lX\r\n", stacked_r12);
    printf("LR = 0x%08lX\r\n", stacked_lr);
    printf("PC = 0x%08lX\r\n", stacked_pc);
    printf("PSR = 0x%08lX\r\n", stacked_psr);
    printf("CFSR = 0x%08lX\r\n", _CFSR);
    printf("HFSR = 0x%08lX\r\n", _HFSR);
    printf("DFSR = 0x%08lX\r\n", _DFSR);
    printf("AFSR = 0x%08lX\r\n", _AFSR);
    printf("BFAR = 0x%08lX\r\n", _BFAR);
    printf("MMAR = 0x%08lX\r\n", _MMAR);

    // 提示信息
    if (_CFSR & (1 << 0)) {
        printf("- INSTRUCTION ACCESS VIOLATION\r\n");
    }
    if (_CFSR & (1 << 1)) {
        printf("- DATA ACCESS VIOLATION at address 0x%08lX\r\n", _BFAR);
    }
    if (_CFSR & (1 << 8)) {
        printf("- UNALIGNED ACCESS\r\n");
    }
    if (_CFSR & (1 << 16)) {
        printf("- UNDEFINED INSTRUCTION\r\n");
    }
    if (_CFSR & (1 << 18)) {
        printf("- INVALID STATE\r\n");
    }
    if (_CFSR & (1 << 19)) {
        printf("- STACK ERROR - Likely STACK OVERFLOW\r\n");
    }
    if (_HFSR & (1 << 30)) {
        printf("- FORCED HARD FAULT - Escalated fault\r\n");
    }

    // 无限循环，防止程序继续执行
    while (1);
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

///**
//  * @brief  This function handles SVCall exception.
//  * @param  None
//  * @retval None
//  */
//void SVC_Handler(void)
//{
//}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

///**
//  * @brief  This function handles PendSVC exception.
//  * @param  None
//  * @retval None
//  */
//void PendSV_Handler(void)
//{
//}

///**
//  * @brief  This function handles SysTick Handler.
//  * @param  None
//  * @retval None
//  */
//void SysTick_Handler(void)
//{
//
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


