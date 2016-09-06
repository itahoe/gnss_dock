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


extern	gnss_fifo_t             fifo_uart1_tx;


/**
 * @brief GNSS initialization
 */
void gnss_init(                             gnss_t *            p )
{
	bsp_mcu_uart1_init( CFG_NMEA_UART_BAUDRATE );

	memset( p, 0, sizeof( gnss_t ) );
}

/**
 * @brief GNSS xmit
 */
void gnss_fifo_write(                           gnss_fifo_t *   p,
                                                uint8_t *       data,
                                                size_t          count )
{
	while( count-- )
	{
		p->data[ p->head++ ]    =   *data++;

		if( p->head >= CFG_GNSS_BLCK_SIZE_OCT )
		{
			p->head                 =   0;
		}
	}
}

/**
 * @brief GNSS send
 */
void gnss_send(                             gnss_t *            gnss,
                                    const   char *              str )
{
	size_t          len;
	uint8_t         chksum;
	gnss_data_t *   xmit        =   &gnss->xmit;
	uint8_t *       xmit_buf    =   xmit->buf[ xmit->pingpong++ & 1 ];


	len	        =   nmea_chksum( &chksum, str, '\0');
	xmit_buf[0]     =   '$';
	memcpy( &xmit_buf[1], str, len++ );
	xmit_buf[len++] =   '*';
	sprintf( (char *) &xmit_buf[len], "%02X\r\n", chksum );
	len             +=  4;

	gnss_fifo_write( &fifo_uart1_tx, xmit_buf, len );
}

/**
 * @brief GNSS recieve char hook
 */
bool gnss_recv_hook(                            gnss_t *            gnss,
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

/**
 * @brief GNSS Recieve 
 */
void gnss_read(                                 gnss_t *            gnss,
                                        const   uint8_t *           data,
                                                size_t              size )
{
	while( size-- )
	{
		gnss_recv_hook( gnss, *data++  );
	}
}

void gnss_recv_start(                           uint8_t *       data,
                                                size_t          size )
{
        bsp_mcu_uart1_recv_start( data, size );
}

/**
 * @brief GNSS UART recieve hook
 */
void gnss_uart_rx_hook(                      gnss_fifo_t *            p )
{
        size_t                  tile    =   p->tile;
        size_t                  size    =   bsp_mcu_uart1_recv_size_get();
        size_t                  head    =   CFG_GNSS_BLCK_SIZE_OCT - size;


	if( head > tile )
	{
		size            =   head - tile;
		p->tile         +=  usb_cdc_send( p->data + tile, size );
	}
	else if( head < tile )
	{
		if( p->overcome )
		{
			p->overcome     = false;
			size            =   CFG_GNSS_BLCK_SIZE_OCT - tile;
			usb_cdc_send( p->data + tile, size );
	                p->tile         =   0;

			#ifndef  NDEBUG
			p->total_overcomes++;
			#endif //NDEBUG
		}
		else
		{
			#ifndef  NDEBUG
			p->total_overruns++;
			#endif //NDEBUG
		}
	}

        #ifndef  NDEBUG
        p->total_data           +=   size;
        #endif //NDEBUG
}
