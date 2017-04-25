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
extern  QueueHandle_t           app_que_usb_cdc_hndl;
extern  QueueHandle_t           app_que_cli_hndl;
extern  QueueHandle_t           app_que_uart1_hndl;
extern  QueueHandle_t           app_que_uart2_hndl;
extern  QueueHandle_t           app_que_uart3_hndl;
        nvg_t                   nvg_usb;
        nvg_t                   nvg_ser3;



static
bool app_task_cli_recv(                         nvg_t *                 p,
                                                char                    c )
{
	bool            resp        =   false;

	switch( c )
	{
		case    '$':
			p->size            =   0;
			break;

		case    '\r':
			resp            =   nvg_recv( p, p->str_inp );
			break;

		case    '\n':
			break;

		default:
			if( p->size < sizeof( p->str_inp ) )
			{
				*(p->str_inp + p->size++)   =   c;
			}

			break;
	}

	return( resp );
}

static
bool app_task_cli_resp(                         nvg_t *                 p )
{
        bool                    resp    =   false;
        uint8_t                 chksum;

        switch( p->type )
        {
                case NVG_TYPE_BSS:
                        break;

                case NVG_TYPE_LOG:
                        app.cfg.log_mode        =   p->log.mode;
                        p->size                 =   snprintf( p->str_out, sizeof(p->str_out), "$PNVGLOG,MODE,%d*", app.cfg.log_mode );
                        nmea_chksum( &chksum, p->str_out+1, '*' );
                        p->size                 +=  snprintf( p->str_out + p->size, sizeof(p->str_out) - p->size, "%02X\r\n", chksum );
                        resp                    =  true;
                        //APP_TRACE( "%s", p->str_out );
                        break;

                case NVG_TYPE_VER:
                        //APP_TRACE( "NVG_TYPE: VER\n" );
                        break;

                default:
                        break;
        }

        return( resp );
}


void app_task_cli(                              void *          argument )
{
        bool                    resp;
        app_pipe_t              pipe;
        app_pipe_t              out;


        nvg_usb.log.mode        =   (nvg_log_mode_t) app_cfg_read( RTC_BKP_DR0 );
        nvg_ser3.log.mode       =   (nvg_log_mode_t) app_cfg_read( RTC_BKP_DR0 );

        while( true )
        {
                xQueueReceive( app_que_cli_hndl, &pipe, portMAX_DELAY );

                switch( pipe.tag )
                {
                        case APP_PIPE_TAG_USB_RECV:
                                while( pipe.size-- )
                                {
                                        resp    =   app_task_cli_recv( &nvg_usb, *pipe.data++ );

                                        if( resp )
                                        {
                                                if( app_task_cli_resp( &nvg_usb ) )
                                                {
                                                        out.tag         =   APP_PIPE_TAG_CLI;
                                                        out.data        =   (uint8_t *) nvg_usb.str_out;
                                                        out.head        =   (uint8_t *) nvg_usb.str_out + nvg_usb.size;
                                                        out.tile        =   (uint8_t *) nvg_usb.str_out;
                                                        out.size        =   nvg_usb.size;
                                                        xQueueSend( app_que_usb_cdc_hndl, &out, portMAX_DELAY );
                                                }
                                        }
                                }
                                break;

                        case APP_PIPE_TAG_UART3:
                                while( pipe.size-- )
                                {
                                        resp    =   app_task_cli_recv( &nvg_ser3, *pipe.data++ );

                                        if( resp )
                                        {
                                                if( app_task_cli_resp( &nvg_ser3 ) )
                                                {
                                                        out.tag         =   APP_PIPE_TAG_CLI;
                                                        out.data        =   (uint8_t *) nvg_ser3.str_out;
                                                        out.head        =   (uint8_t *) nvg_ser3.str_out + nvg_ser3.size;
                                                        out.tile        =   (uint8_t *) nvg_ser3.str_out;
                                                        out.size        =   nvg_ser3.size;
                                                        xQueueSend( app_que_uart3_hndl, &out, portMAX_DELAY );
                                                }
                                        }
                                }
                                break;

                        default:
                                break;
                }

        }
}
