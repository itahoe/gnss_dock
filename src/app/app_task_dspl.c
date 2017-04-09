/**
  * @file    app_task_gnss_dspl.c
  * @brief   GNSS State Display task
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usbd_cdc_interface.h"
#include "gnss.h"
#include "cmsis_os.h"
#include "app.h"
#include "ui.h"


extern  app_fifo_t              ser1_recv;
extern  gnss_t                  gnss;
extern	time_t	time_dat;


static
void app_task_dspl_fix(                         nmea_fix_t              fix )
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


void app_task_dspl(                             void *            arg )
{
        app_fifo_t*             p               =   &ser1_recv;
        uint8_t *               tile            =   p->data;
        uint32_t                dummy;
        bool                    resp;


        while( true )
        {
                xTaskNotifyWait( 0, 0, &dummy, portMAX_DELAY );

                while( tile != p->head )
                {
                        if( tile > (p->data + CFG_GNSS_BLCK_SIZE_OCT) )
                        {
                                tile            =   p->data;
                        }

                        resp    =   gnss_recv( &gnss, *tile++  );

                        if( resp )
                        {
                                app_task_dspl_fix( gnss.nmea.gga.fix );

                                gnss_time_sync( &gnss, &time_dat );
                        }
                }

        }
}
