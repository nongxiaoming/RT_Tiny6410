/*
 * File      : drv_uart.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2009-2013 RT-Thread Develop Team
 *
 * The license and distribution terms for this file may be
 * found in the file LICENSE in this distribution or at
 * http://www.rt-thread.org/license/LICENSE
 *
 * Change Logs:
 * Date           Author       Notes
 * 2013-05-18     Bernard      The first version for LPC40xx
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "serial.h"




static rt_err_t uart_configure(struct rt_serial_device *serial, struct serial_configure *cfg)
{
//     struct lpc_uart *uart;
//     UART_CFG_Type UARTConfigStruct;
//     UART_FIFO_CFG_Type UARTFIFOConfigStruct;
//     
//     RT_ASSERT(serial != RT_NULL);
//     uart = (struct lpc_uart *)serial->parent.user_data;

//     /* Initialize UART Configuration parameter structure to default state:
//      * Baudrate = 115200 bps
//      * 8 data bit
//      * 1 Stop bit
//      * None parity
//      */
//     UART_ConfigStructInit(&UARTConfigStruct);
//     UARTConfigStruct.Baud_rate = 115200;

//     // Initialize UART0 peripheral with given to corresponding parameter
//     UART_Init(uart->UART, &UARTConfigStruct);

    /* Initialize FIFOConfigStruct to default state:
     *              - FIFO_DMAMode = DISABLE
     *              - FIFO_Level = UART_FIFO_TRGLEV0
     *              - FIFO_ResetRxBuf = ENABLE
     *              - FIFO_ResetTxBuf = ENABLE
     *              - FIFO_State = ENABLE
     */
//     UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

//     // Initialize FIFO for UART0 peripheral
//     UART_FIFOConfig(uart->UART, &UARTFIFOConfigStruct);

//     UART_TxCmd(uart->UART, ENABLE);

    return RT_EOK;
}

static rt_err_t uart_control(struct rt_serial_device *serial, int cmd, void *arg)
{
    struct lpc_uart *uart;

    RT_ASSERT(serial != RT_NULL);
    uart = (struct s3c_uart *)serial->parent.user_data;

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* disable rx irq */
      //  UART_IntConfig(uart->UART, UART_INTCFG_RBR, DISABLE);
        break;
    case RT_DEVICE_CTRL_SET_INT:
        /* enable rx irq */
      //  UART_IntConfig(uart->UART, UART_INTCFG_RBR, ENABLE);
        break;
    }

    return RT_EOK;
}

static int lpc_putc(struct rt_serial_device *serial, char c)
{
    struct lpc_uart *uart;

    uart = (struct lpc_uart *)serial->parent.user_data;
   // UART_Send(uart->UART, (uint8_t *)&c, 1, BLOCKING);

    return 1;
}

static int lpc_getc(struct rt_serial_device *serial)
{
    uint8_t ch;
    struct lpc_uart *uart;

    uart = (struct lpc_uart *)serial->parent.user_data;
   // if (UART_Receive(uart->UART, &ch, 1, NONE_BLOCKING) == 1)
     //   return (int) ch;

    return -1;
}

static const struct rt_uart_ops s3c_uart_ops =
{
    uart_configure,
    uart_control,
    lpc_putc,
    lpc_getc,
};

#if defined(RT_USING_UART0)
/* UART0 device driver structure */
#define UART0	((struct uartport *)&U0BASE)
struct serial_int_rx uart0_int_rx;
struct serial_device uart0 =
{
	UART0,
	&uart0_int_rx,
	RT_NULL
};
struct rt_device uart0_device;

/**
 * This function will handle serial
 */
static void rt_serial0_handler(int vector, void *param)
{
	INTSUBMSK |= (BIT_SUB_RXD0);

	rt_hw_serial_isr(&uart0_device);

	SUBSRCPND |= BIT_SUB_RXD0;

	/* Unmask sub interrupt (RXD0) */
	INTSUBMSK  &=~(BIT_SUB_RXD0);
}
#endif

void rt_hw_uart_init(void)
{
    struct lpc_uart *uart;
    struct serial_configure config;

#ifdef RT_USING_UART0
    uart = &uart0;
    config.baud_rate = BAUD_RATE_115200;
    config.bit_order = BIT_ORDER_LSB;
    config.data_bits = DATA_BITS_8;
    config.parity    = PARITY_NONE;
    config.stop_bits = STOP_BITS_1;
    config.invert    = NRZ_NORMAL;

    serial0.ops    = &lpc_uart_ops;
    serial0.int_rx = &uart0_int_rx;
    serial0.config = config;

 	rt_hw_interrupt_install(INTUART0, rt_serial0_handler, RT_NULL, "UART0");
	rt_hw_interrupt_umask(INTUART0);
#endif
}
