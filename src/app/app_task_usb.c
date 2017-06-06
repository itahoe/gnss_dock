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
extern  QueueHandle_t           app_que_uart1_hndl;


void usb_cdc_recv_hook(                 uint8_t *               data,
                                        size_t                  size )
{
        BaseType_t              resp;
        app_pipe_t              pipe    =   {   .tag    =   APP_PIPE_TAG_USB_RECV,
                                                .head   =   data,
                                                .size   =   size        };


        resp    =   xQueueSendFromISR( app_que_uart1_hndl, &pipe, NULL );

        if( resp != pdTRUE )
        {
                APP_TRACE( "%s, usb_cdc_recv_hook : xQueueSendFromISR( app_que_uart1_hndl ) != pdTRUE\n", __FILE__ );
        }


        resp    =   xQueueSendFromISR( app_que_cli_hndl, &pipe, NULL );

        if( resp != pdTRUE )
        {
                APP_TRACE( "%s, usb_cdc_recv_hook : xQueueSendFromISR( app_que_cli_hndl ) != pdTRUE\n", __FILE__ );
        }
}


void app_task_usb(                              void *            argument )
{
        app_pipe_t              pipe;


        (void) argument;

        USBD_Init(                      &husbd, &VCP_Desc,      0);
        USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
        USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
        USBD_Start( &husbd );

        while( true )
        {
                if( xQueueReceive( app_que_usb_cdc_hndl, &pipe, portMAX_DELAY ) )
                {
                        switch( pipe.tag )
                        {
                                case APP_PIPE_TAG_CLI:
                                        usb_cdc_xmit( pipe.head, pipe.size );
                                        break;

                                case APP_PIPE_TAG_UART1:
                                        usb_cdc_xmit( pipe.head, pipe.size );
                                        break;

                                default:
                                        break;
                        }
                }
        }
}
