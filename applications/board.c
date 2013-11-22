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


/**
 * This is the timer interrupt service routine.
 *
 */
void Timer0_Handler(void)
{
    /* enter interrupt */
   // rt_interrupt_enter();

    //rt_tick_increase();

    /* leave interrupt */
   // rt_interrupt_leave();
}
void rt_timer_hw_init()
{
 VIC0VECTADDR(23)=(volatile unsigned long)Timer0_Handler;//�жϷ��������
	 TCFG0 =65;   			// ���÷�Ƶ����(66��Ƶ), ��ʱ��ʱ��Ƶ��Ϊ1Mhz
	 TCNTB0 = RT_TICK_PER_SECOND*1000;     //���ó�ֵ������TCTB0
	 TCON |= 1<<1;      	//���ó�ֵ��Ҫ����TCNTB
	 TCON |= 1<<3;     	//�Զ���װ����
	 TCON &= ~(1<<1);   	//����Update TCNTB0,TCMPB0
	 TCON |= 1<<0;       	//timer0 open;
	 TINT_CSTAT=1<<5;			//�����ʱ��״̬λ
	 TINT_CSTAT |= 1<<0;       //��timer0�ж�,����timer0�жϷ���
	 VIC0INTENABLE |= 1<<23;   //��timer0��ʹ��
//	VIC0VECTADDR(23)=(volatile unsigned long)Timer0_Handler;//�жϷ��������
//	 TCFG0 =65;   			// ���÷�Ƶ����(66��Ƶ), ��ʱ��ʱ��Ƶ��Ϊ1Mhz
//	 TCNTB0 = 1000*1000;     //���ó�ֵ������TCTB0
//	 TCON |= 1<<1;      	//���ó�ֵ��Ҫ����TCNTB
//	 TCON |= 1<<3;     	//�Զ���װ����
//	 TCON &= ~(1<<1);   	//����Update TCNTB0,TCMPB0
//	 TCON |= 1<<0;       	//timer0 open;
//	 TINT_CSTAT=1<<5;			//�����ʱ��״̬λ
//	 TINT_CSTAT |= 1<<0;       //��timer0�ж�,����timer0�жϷ���
//	 VIC0INTENABLE |= 1<<23;   //��timer0��ʹ��
}
/**
 * This function will initial LPC17xx board.
 */
void rt_hw_board_init()
{
	/* initialize led port */
	rt_hw_led_init();

	/* initialize uart */
	rt_hw_uart_init();

	/* initialize mmu */
	rt_hw_mmu_init();
	
	/* initialize timer0 */
   rt_timer_hw_init();
       //rt_hw_uart_init();
//	     while(1)
//				 {
//          //GPIOK->DAT^=(0x01<<4);
//					 i=500000;
//					 while(i--);
//        }
//     rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
//     rt_led_hw_init();

}
