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
extern  QueueHandle_t           app_que_gnss_hndl;
extern  QueueHandle_t           app_que_dspl_hndl;
extern  gnss_t                  gnss;
extern	time_t	time_dat;


static
void app_task_gnss_dspl(                        nmea_fix_t              fix )
{
        switch( fix )
        {
                case NMEA_FIX_RTK_FLT:	ui_led_gnss_set( UI_LED_GNSS_MODE_RTKFLT ); break;
                case NMEA_FIX_RTK_INT:  ui_led_gnss_set( UI_LED_GNSS_MODE_RTKINT ); break;
                case NMEA_FIX_PPS:      ui_led_gnss_set( UI_LED_GNSS_MODE_DGPS   ); break;
                case NMEA_FIX_DGPS:     ui_led_gnss_set( UI_LED_GNSS_MODE_DGPS   ); break;
                case NMEA_FIX_SPS:      ui_led_gnss_set( UI_LED_GNSS_MODE_GPS    ); break;
                case NMEA_FIX_NONE:     ui_led_gnss_set( UI_LED_GNSS_MODE_NONE   ); break;
                default:                ui_led_gnss_set( UI_LED_GNSS_MODE_NONE   ); break;
        }
}




void app_task_gnss(                             void *            argument )
{
        bool                    resp;
        app_pipe_t              pipe;
        size_t                  timeout_msec    =   100;
        TickType_t              timeout_tcks    =   timeout_msec / portTICK_PERIOD_MS;


        (void) argument;


        while( true )
        {
                if( xQueueReceive( app_que_gnss_hndl, &pipe, timeout_tcks ) )
                {
                        switch( pipe.tag )
                        {
                                case APP_PIPE_TAG_UART1:
                                        while( pipe.size-- )
                                        {
                                                resp    =   gnss_recv( &gnss, *pipe.data++  );

                                                if( resp )
                                                {
                                                        app_task_gnss_dspl( gnss.nmea.gga.fix );
                                                        gnss_time_sync( &gnss, &time_dat );
                                                }
                                        }

                                        break;

                                default:
                                        break;
                        }
                }
        }
}
