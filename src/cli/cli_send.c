/**
 * @file cli_send.c
 * @brief	Command Line Interface
 * @author  Igor T. <research.tahoe@gmail.com>
 */


#include <stdlib.h>
#include "cli.h"


/**
 * @brief CLI transmit.
 */
void cli_send(                                  char *                  data )
{
	while( *data != '\0' )
    {
        bsp_uart_send( *data++ );
	}
}
