/*
 * realview-logger.c
 *
 * Copyright(c) 2007-2015 Jianjun Jiang <8192542@qq.com>
 * Official site: http://xboot.org
 * Mobile phone: +86-18665388956
 * QQ: 8192542
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <xboot.h>
#include <realview/reg-uart.h>

static void logger_uart0_init(void)
{
	writel(REALVIEW_UART0_BASE + REALVIEW_UART_OFFSET_CR, 0x0);
	writel(REALVIEW_UART0_BASE + REALVIEW_UART_OFFSET_CR, REALVIEW_UART_CR_UARTEN |	REALVIEW_UART_CR_TXE | REALVIEW_UART_CR_RXE);
}

static void logger_uart0_exit(void)
{
}

static ssize_t logger_uart0_output(const char * buf, size_t count)
{
	ssize_t i;

	for(i = 0; i < count; i++)
	{
		while( (readb(REALVIEW_UART0_BASE + REALVIEW_UART_OFFSET_FR) & REALVIEW_UART_FR_TXFF) );
		writeb(REALVIEW_UART0_BASE + REALVIEW_UART_OFFSET_DATA, buf[i]);
	}
	return i;
}

static struct logger_t realview_logger = {
	.name	= "logger-uart0",
	.init	= logger_uart0_init,
	.exit	= logger_uart0_exit,
	.output	= logger_uart0_output,
};

static __init void realview_logger_init(void)
{
	if(register_logger(&realview_logger))
		LOG("Register logger '%s'", realview_logger.name);
	else
		LOG("Failed to register logger '%s'", realview_logger.name);
}

static __exit void realview_logger_exit(void)
{
	if(unregister_logger(&realview_logger))
		LOG("Unregister logger '%s'", realview_logger.name);
	else
		LOG("Failed to unregister logger '%s'", realview_logger.name);
}

pure_initcall(realview_logger_init);
pure_exitcall(realview_logger_exit);
