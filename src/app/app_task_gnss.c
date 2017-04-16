/**
  * @file    app_task_gnss.c
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "usbd_cdc_interface.h"
#include "gnss.h"
#include "cmsis_os.h"
#include "app.h"

#include "ui.h"


        gnss_t                  gnss;
extern  app_fifo_t              ser1_recv;
extern  app_fifo_t              ser2_recv;
extern  app_fifo_t              ser3_recv;
        app_fifo_t              ser3_xmit;
extern  QueueHandle_t           app_que_gnss_hndl;


static
void app_task_gnss_ser3_send_circular(          app_stream_t *          p )
{
        size_t          len;


        if( p->tile > p->head )
        {
                len             =   (p->data + CFG_GNSS_BLCK_SIZE_OCT) - p->tile;
                gnss_ser3_xmit( p->tile, len );
                p->tile         =   p->data;
        }

        len             =   p->head - p->tile;

        if( len > 0 )
        {
                gnss_ser3_xmit( p->tile, len );
                p->tile         +=  len;
        }
}

/*
static
void app_task_gnss_ser2_send_circular(          app_stream_t *          p )
{
        size_t          len;


        if( p->tile > p->head )
        {
                len             =   (p->data + CFG_GNSS_BLCK_SIZE_OCT) - p->tile;
                gnss_ser2_xmit( p->tile, len );
                p->tile         =   p->data;
        }

        len             =   p->head - p->tile;

        if( len > 0 )
        {
                gnss_ser2_xmit( p->tile, len );
                p->tile         +=  len;
        }
}
*/

void app_task_gnss(                             void *            argument )
{
        app_stream_t            stream;
        size_t                  timeout_msec    =   100;
        TickType_t              timeout_tcks    =   timeout_msec / portTICK_PERIOD_MS;


        (void) argument;

	//gnss_init( &gnss );

	//gnss_ser3_xmit( ser3_xmit.data, CFG_GNSS_BLCK_SIZE_OCT );

	//gnss_ser3_recv_start( ser3_recv.data, CFG_GNSS_BLCK_SIZE_OCT );
	//gnss_ser2_recv_start( ser2_recv.data, CFG_GNSS_BLCK_SIZE_OCT );
	//gnss_ser1_recv_start( ser1_recv.data, CFG_GNSS_BLCK_SIZE_OCT );

        while( true )
        {
                //if( xQueueReceive( app_que_gnss_hndl, &stream, portMAX_DELAY ) )
                if( xQueueReceive( app_que_gnss_hndl, &stream, timeout_tcks ) )
                {
                        switch( stream.type )
                        {
                                case APP_MSG_TYPE_SER3_RECV:
                                        //app_task_gnss_ser2_send_circular( &stream );
                                        APP_TRACE( "%s\n", stream.head );
                                        break;

                                case APP_MSG_TYPE_SER2_RECV:
                                        app_task_gnss_ser3_send_circular( &stream );
                                        break;

                                case APP_MSG_TYPE_SER1_RECV:
                                case APP_MSG_TYPE_USB_RECV:
                                case APP_MSG_TYPE_ERROR:
                                default:
                                        break;
                        }
                }

                //ui_led_pwr_set(         UI_LED_RGB_COLOR_RED    );
                //ui_led_pwr_set(         UI_LED_RGB_COLOR_GREEN  );

                //xQueueReceive( app_que_gnss_hndl, &stream, timeout_tcks );
/*
                size    =   gnss_ser3_recv();

                if( size )
                {
                        app_task_gnss_ser2_send_circular();
                }
*/
        }
}
