/**
  * @file    gnss.c
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usbd_cdc_interface.h"
#include "bsp_gnss.h"
#include "gnss.h"


//uint8_t                 gnss_data_recv[ CFG_GNSS_BLCK_SIZE_OCT ];
//uint8_t                 gnss_data_xmit[ CFG_GNSS_BLCK_SIZE_OCT ];

gnss_fifo_t             gnss_data_uart_rx;
gnss_fifo_t             gnss_data_uart_tx;



/**
 * @brief GNSS initialization
 */
void gnss_init(                             gnss_t *            p )
{
	bsp_gnss_init();

	memset( p, 0, sizeof( gnss_t ) );
}

/**
 * @brief GNSS xmit
 */
void gnss_xmit(                                 gnss_fifo_t *   p,
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
	gnss_smbl_t *   xmit_buf    =   xmit->buf[ xmit->pingpong++ & 1 ];


	len	        =   nmea_chksum( &chksum, str, '\0');

	xmit_buf[0]     =   '$';
	memcpy( &xmit_buf[1], str, len++ );
	xmit_buf[len++] =   '*';
	sprintf( (char *) &xmit_buf[len], "%02X\r\n", chksum );
	len             +=  4;

	gnss_xmit( &gnss_data_uart_tx, xmit_buf, len );
}

/**
 * @brief GNSS recieve char hook
 */
bool gnss_recv_hook(                            gnss_t *            gnss,
                                        const   char                c )
{
	bool            resp        =   false;
	gnss_data_t *   recv        =   &gnss->recv;
	gnss_smbl_t *   recv_buf    =   recv->buf[ 0 ];


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

/**
 * @brief UART Control
 */
void gnss_ctl(                                  gnss_ctl_t          ctl )
{
	switch( ctl )
	{
		case GNSS_CTL_RECV_START:    bsp_gnss_recv_start( gnss_data_uart_rx.data, CFG_GNSS_BLCK_SIZE_OCT ); break;
		case GNSS_CTL_RECV_STOP:     bsp_gnss_recv_stop(); break;
		case GNSS_CTL_XMIT_START:    gnss_xmit( &gnss_data_uart_tx, gnss_data_uart_rx.data, CFG_GNSS_BLCK_SIZE_OCT ); break;
		case GNSS_CTL_XMIT_STOP:     bsp_gnss_xmit_stop(); break;
		default:                        break;

		//case GNSS_CTL_RECV_START:    HAL_UART_Receive_DMA( &huart, (uint8_t *) gnss_data_recv, CFG_GNSS_BLCK_SIZE_OCT );	break;
		//case GNSS_CTL_XMIT_START:    HAL_UART_Transmit_DMA( &huart, (uint8_t *) gnss_data_xmit, CFG_GNSS_BLCK_SIZE_OCT );	break;
		//case GNSS_CTL_RECV_START:    bsp_gnss_recv_start( gnss_data_recv, CFG_GNSS_BLCK_SIZE_OCT ); break;
		//case GNSS_CTL_XMIT_START:    bsp_gnss_xmit( gnss_data_xmit, CFG_GNSS_BLCK_SIZE_OCT ); break;
	}
}

/**
 * @brief GNSS UART recieve hook
 */

void gnss_uart_rx_hook(                      gnss_fifo_t *            p )
{
        //gnss_fifo_t *           fifo    =   &( p->cdc );
        size_t                  tile    =   p->tile;
        size_t                  size    =   bsp_gnss_recv_fifo_size_get();
        size_t                  head    =   CFG_GNSS_BLCK_SIZE_OCT - size;


	if( head > tile )
	{
		size            =   head - tile;
		p->tile         +=  usb_cdc_send( gnss_data_uart_rx.data + tile, size );
	}
	else if( head < tile )
	{
		if( p->overcome )
		{
			p->overcome     = false;
			size            =   CFG_GNSS_BLCK_SIZE_OCT - tile;
			usb_cdc_send( gnss_data_uart_rx.data + tile, size );
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
