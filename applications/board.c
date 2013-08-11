/*
 * File      : board.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 * 2010-02-04     Magicoe      ported to LPC17xx
 * 2010-05-02     Aozima       update CMSIS to 130
 */

#include <rthw.h>
#include <rtthread.h>

#include "board.h"
#include "drv_uart.h"
//#include "drv_lcd.h"
#ifdef LPC_EXT_SDRAM
//#include "drv_sdram.h"
#endif

/**
 * This is the timer interrupt service routine.
 *
 */
void Timer0_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();
}
void rt_timer_hw_init()
{
 VIC0VECTADDR(23)=(volatile unsigned long)Timer0_Handler;//中断服务函数入口
	 TCFG0 =65;   			// 设置分频因子(66分频), 定时器时钟频率为1Mhz
	 TCNTB0 = (1000/RT_TICK_PER_SECOND)*1000;     //设置初值，用于TCTB0
	 TCON |= 1<<1;      	//设置初值后要更新TCNTB
	 TCON |= 1<<3;     	//自动重装开启
	 TCON &= ~(1<<1);   	//不再Update TCNTB0,TCMPB0
	 TCON |= 1<<0;       	//timer0 open;
	 TINT_CSTAT=1<<5;			//清除定时器状态位
	 TINT_CSTAT |= 1<<0;       //开timer0中断,允许timer0中断发生
	 VIC0INTENABLE |= 1<<23;   //开timer0的使能
}
/**
 * This function will initial LPC17xx board.
 */
void rt_hw_board_init()
{
//     /* NVIC Configuration */
// #define NVIC_VTOR_MASK              0x3FFFFF80
// #ifdef  VECT_TAB_RAM
//     /* Set the Vector Table base location at 0x10000000 */
//     SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
// #else  /* VECT_TAB_FLASH  */
//     /* Set the Vector Table base location at 0x00000000 */
//     SCB->VTOR  = (0x00000000 & NVIC_VTOR_MASK);
// #endif

    /* init systick */
//     SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND - 1);
//     /* set pend exception priority */
//     NVIC_SetPriority(PendSV_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
       rt_timer_hw_init();
//     rt_hw_uart_init();
//     rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
//     rt_led_hw_init();
#if LPC_EXT_SDRAM == 1
    rt_kprintf("Initialize SDRAM ...");
    lpc_sdram_hw_init();
    rt_kprintf("done!\n");
#endif
}
