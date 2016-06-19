/**
  * @file    gnss.c
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>
#include	"bsp_gnss.h"
#include	"gnss.h"


	char                    uart_data_xmit[ CFG_FMNG_BLCK_SIZE_OCT ];
	char                    uart_data_recv[ CFG_FMNG_BLCK_SIZE_OCT ];
	char                    nmea_str[ NMEA_STRLEN_MAX_OCT ];

	size_t                  uart_data_xmit_sizeof      =   sizeof( uart_data_xmit );
	size_t                  uart_data_recv_sizeof      =   sizeof( uart_data_recv );

extern	UART_HandleTypeDef      huart;


/**
 * @brief GNSS initialization
 */
void gnss_init(                             gnss_t *            gnss )
{
	memset( gnss, 0, sizeof( gnss_t ) );
	bsp_gnss_init();
}

/**
 * @brief GNSS send
 */
void gnss_send(                             gnss_t *            gnss,
                                    const   char *              str )
{
	size_t          len;
	uint8_t         chksum;
	gnss_xfer_t *	xmit        =   &gnss->xmit;
	char *          xmit_buf    =   xmit->buf[ xmit->pingpong++ & 1 ];


	len	        =   nmea_chksum( &chksum, str, '\0');

	xmit_buf[0]     =   '$';
	memcpy( &xmit_buf[1], str, len++ );
	xmit_buf[len++] =   '*';
	sprintf( &xmit_buf[len], "%02X\r\n", chksum );
	len             +=  4;
	bsp_gnss_xmit( xmit_buf, len );
}

/**
 * @brief GNSS recieve char hook
 */
bool gnss_recv_hook(                        gnss_t *            gnss,
                                    const   char                c )
{
	bool            resp        =   false;
	gnss_xfer_t *	recv        =   &gnss->recv;
	char *          recv_buf    =   recv->buf[ 0 ];


	switch( c )
	{
		case    '$':
			recv->len       =   0;
			break;

		case    '\n':
			resp            =   nmea_recv( &gnss->nmea, recv_buf );
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
void gnss_recv(                             gnss_t *            gnss,
                                    const   uint8_t *           str,
	                                    size_t              size )
{
	while( size-- )
	{
		gnss_recv_hook( gnss, *str++  );
	}
}

/**
 * @brief UART Control
 */
void	gnss_ctl(                           gnss_ctl_t          ctl )
{
	switch( ctl )
	{
		case GNSS_CTL_RECV_START:    HAL_UART_Receive_DMA( &huart, (uint8_t *) uart_data_recv, CFG_FMNG_BLCK_SIZE_OCT );	break;
		case GNSS_CTL_RECV_STOP:     ;	break;
		case GNSS_CTL_XMIT_START:    HAL_UART_Transmit_DMA( &huart, (uint8_t *) uart_data_xmit, CFG_FMNG_BLCK_SIZE_OCT );	break;
		case GNSS_CTL_XMIT_STOP:     ;	break;
		default:                        break;
	}
}

