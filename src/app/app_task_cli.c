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
//#include "nmea.h"


extern  app_t                   app;
extern  QueueHandle_t           app_que_cli_hndl;
extern  QueueHandle_t           app_que_storage_hndl;
extern  QueueHandle_t           app_que_uart1_hndl;
extern  QueueHandle_t           app_que_uart2_hndl;
extern  QueueHandle_t           app_que_uart3_hndl;
extern  QueueHandle_t           app_que_usb_cdc_hndl;
        nvg_t                   nvg_usb;
        nvg_t                   nvg_ser3;

/*
static
bool app_task_cli_resp(                         nvg_t *                 p,
                                        const   nvg_type_t              type )
{
        bool                    resp    =   false;
        uint8_t                 chksum;

        switch( type )
        {
                case NVG_TYPE_INVALID:
                        break;

                case NVG_TYPE_BSS:
                        break;

                case NVG_TYPE_LOG_MODE:
                        app.cfg.log_mode        =   p->log.mode;
                        p->size                 =   snprintf( p->str_out, sizeof(p->str_out), "$PNVGLOG,MODE,%d*", app.cfg.log_mode );
                        nmea_chksum( &chksum, p->str_out+1, '*' );
                        p->size                 +=  snprintf( p->str_out + p->size, sizeof(p->str_out) - p->size, "%02X\r\n", chksum );
                        resp                    =  true;
                        break;

                case NVG_TYPE_LOG_CLOSE:
                        break;

                case NVG_TYPE_LOG_OPEN:
                        break;

                case NVG_TYPE_LOG_RESTART:
                        break;

                case NVG_TYPE_VER:
                        break;

                default:
                        break;
        }

        return( resp );
}
*/

/*
static
nvg_type_t app_task_cli_getc(                   nvg_t *                 p,
                                                char                    c )
{
        nvg_type_t      type    =   NVG_TYPE_INVALID;


        if(      c == '$' )
        {
                p->size =   0;
        }
        else if( c == '\r' )
        {
                type    =   nvg_recv( p, p->str_inp );
        }
        else if( c == '\n' )
        {
        }
        else if( p->size < sizeof( p->str_inp ) )
        {
                *(p->str_inp + p->size++)   =   c;
        }

	return( type );
}
*/

static
void app_task_cli_recv(                         nvg_t *                 p,
                                                app_pipe_t *            inp,
                                                QueueHandle_t           que )
{
        app_pipe_t              out;
        nvg_type_t              type;
        bool                    resp;


        while( inp->size-- )
        {
                type    =   nvg_getc( p, *inp->head++ );

                switch( type )
                {
                        case NVG_TYPE_INVALID:
                        //case NVG_TYPE_BSS:
                        //case NVG_TYPE_VER:
                                break;
/*
                        case NVG_TYPE_LOG_MODE:
                                app.cfg.log_mode        =   p->log.mode;
                                nvg_resp( p, (nvg_log_mode_t) app.cfg.log_mode );

                                out.tag                 =   APP_PIPE_TAG_CLI;
                                out.head                =   (uint8_t *) p->str_out;
                                out.size                =   p->size;
                                xQueueSend( que, &out, portMAX_DELAY );
                                break;
*/
                        case NVG_TYPE_LOG_MODE_GET:
                                nvg_resp( p, (nvg_log_mode_t) app.cfg.log_mode );
                                out.tag                 =   APP_PIPE_TAG_CLI;
                                out.head                =   (uint8_t *) p->str_out;
                                out.size                =   p->size;
                                xQueueSend( que, &out, portMAX_DELAY );
                                break;

                        case NVG_TYPE_LOG_MODE_SET:
                                app.cfg.log_mode        =   p->log.mode;
                                break;

                        case NVG_TYPE_LOG_CLOSE:
                                out.tag                 =   APP_PIPE_TAG_STORAGE_CLOSE;

                                do
                                {
                                        resp    =   xQueueSend( app_que_storage_hndl, &out, NULL );
                                }
                                while( resp != pdTRUE );
                                break;

                        case NVG_TYPE_LOG_OPEN:
                                out.tag                 =   APP_PIPE_TAG_STORAGE_OPEN;

                                do
                                {
                                        resp    =   xQueueSend( app_que_storage_hndl, &out, NULL );
                                }
                                while( resp != pdTRUE );
                                break;

                        case NVG_TYPE_LOG_RESTART:
                                out.tag                 =   APP_PIPE_TAG_STORAGE_OPEN;

                                do
                                {
                                        resp    =   xQueueSend( app_que_storage_hndl, &out, NULL );
                                }
                                while( resp != pdTRUE );
                                break;

                        default:
                                break;
                }
        }
}


void app_task_cli(                              void *          argument )
{
        app_pipe_t              pipe;


        nvg_usb.log.mode        =   (nvg_log_mode_t) app_cfg_read( RTC_BKP_DR0 );
        nvg_ser3.log.mode       =   (nvg_log_mode_t) app_cfg_read( RTC_BKP_DR0 );

        while( true )
        {
                xQueueReceive( app_que_cli_hndl, &pipe, portMAX_DELAY );

                switch( pipe.tag )
                {
                        case APP_PIPE_TAG_USB_RECV:
                                app_task_cli_recv( &nvg_usb, &pipe, app_que_usb_cdc_hndl );
                                break;

                        case APP_PIPE_TAG_UART3:
                                app_task_cli_recv( &nvg_ser3, &pipe, app_que_uart3_hndl );
                                break;

                        default:
                                break;
                }
        }
}
