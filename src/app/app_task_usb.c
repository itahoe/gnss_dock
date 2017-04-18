/**
  * @file    app_task_usb.c
  * @brief   USB Task
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "cmsis_os.h"
#include "app.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


        USBD_HandleTypeDef      husbd;
extern  QueueHandle_t           app_que_usb_cdc_hndl;
extern  QueueHandle_t           app_que_cli_hndl;
extern  QueueHandle_t           app_que_comm_hndl;


static
void app_task_usb_send_circular(                app_stream_t *          p )
{
        size_t          len;


        if( p->tile > p->head )
        {
                len             =   (p->data + CFG_GNSS_BLCK_SIZE_OCT) - p->tile;
                usb_cdc_xmit( p->tile, len );
                p->tile         =   p->data;
        }

        len             =   p->head - p->tile;

        if( len > 0 )
        {
                usb_cdc_xmit( p->tile, len );
                p->tile         +=  len;
        }
}


void usb_cdc_recv_hook(                 uint8_t *               data,
                                        size_t                  size )
{
        BaseType_t              resp;
        app_stream_t            stream  =   {   .type   =   APP_MSG_TYPE_USB_RECV,
                                                .data   =   data,
                                                .size   =   size        };

        resp    =   xQueueSendFromISR( app_que_cli_hndl, &stream, NULL );
        resp    =   xQueueSendFromISR( app_que_comm_hndl, &stream, NULL );

        if( resp != pdTRUE )
        {
                //queue send error
        }
}


void app_task_usb(                              void *            argument )
{
        app_stream_t            stream;


        (void) argument;

        USBD_Init(                      &husbd, &VCP_Desc,      0);
        USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
        USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
        USBD_Start( &husbd );

        while( true )
        {
                if( xQueueReceive( app_que_usb_cdc_hndl, &stream, portMAX_DELAY ) )
                {
                        switch( stream.type )
                        {

                                case APP_MSG_TYPE_CLI_XMIT:
                                        APP_TRACE( "app_task_usb::app_usb_cdc_xmit_que_hndl\n" );
                                        app_task_usb_send_circular( &stream );
                                        break;

                                case APP_MSG_TYPE_SER1_RECV:
                                        app_task_usb_send_circular( &stream );
                                        break;

                                case APP_MSG_TYPE_USB_RECV:
                                case APP_MSG_TYPE_SER3_RECV:
                                case APP_MSG_TYPE_SER2_RECV:
                                case APP_MSG_TYPE_ERROR:
                                default:
                                        break;
                        }
                }
        }
}
