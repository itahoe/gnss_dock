/**
  * @file    app_task_cli.c
  * @brief   Command Line Interface
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "cmsis_os.h"
#include "config.h"
#include "app.h"
#include "nvg.h"
#include "usbd_cdc_interface.h"
#include "nmea.h"


extern  app_t                   app;
extern  app_fifo_t              gnss_com1_incoming;
extern  QueueHandle_t           app_que_usb_cdc_hndl;
extern  QueueHandle_t           app_que_cli_hndl;
extern  QueueHandle_t           app_que_comm_hndl;
        nvg_t                   nvg;

static  size_t                  size;
static  char                    str_inp[80];
static  char                    str_out[80];


static
bool app_task_cli_recv(                         nvg_t *                 p,
                                                char                    c )
{
	bool            resp        =   false;

	switch( c )
	{
		case    '$':
			size            =   0;
			break;

		case    '\n':
			resp            =   nvg_recv( p, str_inp );
			break;

		case    '\r':
			break;

		default:
			if( size < sizeof( str_inp ) )
			{
				*(str_inp + size++)   =   c;
			}

			break;
	}

	return( resp );
}

static
void app_task_cli_resp(                         nvg_t *                 p )
{
        BaseType_t              resp;
        size_t                  len;
        app_stream_t            stream;
        uint8_t                 chksum;

        switch( p->type )
        {
                case NVG_TYPE_BSS:
                        //APP_TRACE( "NVG_TYPE: BSS\n" );
                        break;

                case NVG_TYPE_LOG:
                        app.cfg.log_mode        =   p->log.mode;
                        len                     =   snprintf( str_out, sizeof(str_out), "$PNVGLOG,MODE,%d*", app.cfg.log_mode );
                        nmea_chksum( &chksum, str_out+1, '*' );
                        len                     +=  snprintf( str_out+len, sizeof(str_out)-len, "%02X\r\n", chksum );

                        APP_TRACE( "app_task_cli_resp: %s\n", str_out );

                        stream.type             =   APP_MSG_TYPE_CLI_XMIT;
                        stream.data             =   (uint8_t *) str_out;
                        stream.head             =   (uint8_t *) str_out + len;
                        stream.tile             =   (uint8_t *) str_out;
                        stream.size             =   len;

                        //resp    =   xQueueSendFromISR( app_usb_cdc_xmit_que_hndl, &stream, NULL );
                        resp    =   xQueueSend( app_que_usb_cdc_hndl, &stream, portMAX_DELAY );
                        resp    =   xQueueSend( app_que_comm_hndl, &stream, portMAX_DELAY );

                        if( resp != pdTRUE )
                        {
                                //queue send error
                        }
                        break;

                case NVG_TYPE_VER:
                        //APP_TRACE( "NVG_TYPE: VER\n" );
                        break;

                default:
                        break;
        }
}


void app_task_cli(                              void *          argument )
{
        bool                    resp;
        app_stream_t            stream;


        nvg.log.mode    =   (nvg_log_mode_t) app_cfg_read( RTC_BKP_DR0 );

        while( true )
        {
                xQueueReceive( app_que_cli_hndl, &stream, portMAX_DELAY );

                switch( stream.type )
                {
                        case APP_MSG_TYPE_USB_RECV:
                                while( stream.size-- )
                                {
                                        resp    =   app_task_cli_recv( &nvg, *stream.data++ );

                                        if( resp )
                                        {
                                                app_task_cli_resp( &nvg );
                                        }
                                }
                                break;

                        case APP_MSG_TYPE_SER3_RECV:
                        case APP_MSG_TYPE_SER2_RECV:
                        case APP_MSG_TYPE_SER1_RECV:
                        case APP_MSG_TYPE_ERROR:
                        default:
                                break;
                }

        }
}
