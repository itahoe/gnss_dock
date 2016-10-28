/**
 * @file cli.c
 * @brief	Command Line Interface
 * @author  Igor T. <research.tahoe@gmail.com>
 */


#include <stdlib.h>
//#include "bsp_uart.h"
#include "cli.h"

/**
 * @brief CLI initialization.
 */
void cli_init(                                  cli_t *                 p )
{
	memset( p, 0, sizeof(cli_t) );
	bsp_uart_init();
}
