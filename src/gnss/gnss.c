/**
  * @file    gnss.c
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usbd_cdc_interface.h"
#include "bsp_mcu.h"
#include "gnss.h"
#include "ui.h"


//extern  fifo_t                  fifo_uart1_tx;


/**
 * @brief GNSS initialization
 */
void gnss_init(                             gnss_t *            p )
{
	memset( p, 0, sizeof( gnss_t ) );
	bsp_mcu_uart1_init( CFG_NMEA_UART_BAUDRATE );
	//bsp_mcu_uart2_init( CFG_NMEA_UART_BAUDRATE );
	bsp_mcu_uart2_init( 230400 );
	//bsp_mcu_uart3_init( 115200 );
}

void gnss_ser3_init(                    size_t                  baud    )
{
	bsp_mcu_uart3_init( baud );
}

void gnss_ser3_recv_start(                      uint8_t *       data,
                                                size_t          size    )
{
        bsp_mcu_uart3_recv_start( data, size );
}

void gnss_ser2_recv_start(                      uint8_t *       data,
                                                size_t          size )
{
        bsp_mcu_uart2_recv_start( data, size );
}

void gnss_ser1_recv_start(                      uint8_t *       data,
                                                size_t          size )
{
        bsp_mcu_uart1_recv_start( data, size );
}

/*
size_t gnss_ser3_recv(                          void                    )
{

}
*/

size_t gnss_ser3_xmit(                          uint8_t *               data,
                                                size_t                  size )
{
        size_t          sent    =   0;

        bsp_mcu_uart3_xmit_start( data, size );

        return( sent );
}

size_t gnss_ser2_xmit(                          uint8_t *               data,
                                                size_t                  size )
{
        size_t          sent    =   0;

        bsp_mcu_uart2_xmit_start( data, size );

        return( sent );
}


/**
 * @brief GNSS recieve
 */
bool gnss_recv(                                 gnss_t *            gnss,
                                        const   char                c )
{
	bool            resp        =   false;
	gnss_data_t *   recv        =   &gnss->recv;
	uint8_t *       recv_buf    =   recv->buf[ 0 ];


	switch( c )
	{
		case    '$':
			recv->len       =   0;
			break;

		case    '\n':
			resp            =   nmea_recv( &gnss->nmea, (char *) recv_buf );
			break;

		case    '\r':
			break;

		default:
			if( recv->len < NMEA_STRLEN_MAX_OCT-1 )
			{
				*(recv_buf + recv->len++)   =   c;
			}
			break;
	}

	return( resp );
}
