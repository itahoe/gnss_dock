/**
  * @file    app_task_storage.c
  * @brief   Storage service
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <string.h>
#include "cmsis_os.h"
#include "app.h"
#include "config.h"
#include "storage.h"
#include "ui.h"
#include "nvg.h"


extern  app_t                   app;
extern  QueueHandle_t           app_que_storage_hndl;
static  storage_t               log_com1        =   { .fext   =  "ser1.log", };
static  storage_t               log_com2        =   { .fext   =  "ser2.log", };


void app_task_storage(                          void *          argument )
{
        app_pipe_t              pipe;
        bool                    write_uart2_active      =   false;
        bool                    write_uart1_active      =   false;
        const   TickType_t      fopen_err_delay_msec    = 100;


	storage_init();

        while( true )
        {
                xQueueReceive( app_que_storage_hndl, &pipe, portMAX_DELAY );

                switch( pipe.tag )
                {
                        case APP_PIPE_TAG_STORAGE_TOGGLE:
                                if( write_uart2_active || write_uart1_active )
                                {
                                        write_uart2_active      =   false;
                                        write_uart1_active      =   false;

                                        switch( app.cfg.log_mode )
                                        {
                                                case NVG_LOG_MODE_3:
                                                        storage_close( &log_com2 );
                                                        storage_close( &log_com1 );
                                                        break;

                                                case NVG_LOG_MODE_2:
                                                        storage_close( &log_com2 );
                                                        break;

                                                case NVG_LOG_MODE_1:
                                                        storage_close( &log_com1 );
                                                        break;

                                                case NVG_LOG_MODE_INVALID:
                                                default:
                                                        break;
                                        }

                                }
                                else
                                {
                                        switch( app.cfg.log_mode )
                                        {
                                                case NVG_LOG_MODE_3:
                                                        write_uart2_active      =   storage_open( &log_com2 );
                                                        write_uart1_active      =   storage_open( &log_com1 );
                                                        break;

                                                case NVG_LOG_MODE_2:
                                                        write_uart2_active      =   storage_open( &log_com2 );
                                                        break;

                                                case NVG_LOG_MODE_1:
                                                        write_uart1_active      =   storage_open( &log_com1 );
                                                        break;

                                                case NVG_LOG_MODE_INVALID:
                                                default:
                                                        break;
                                        }

                                        if( !write_uart2_active && !write_uart1_active )
                                        {
                                                ui_led_sd_set( true );
                                                osDelay( fopen_err_delay_msec );
                                        }
                                }

                                ui_led_sd_set(  write_uart2_active || write_uart1_active );

                                break;

                        case APP_PIPE_TAG_UART2:
                                if( write_uart2_active )
                                {
                                        ui_led_sd_set(  false );
                                        storage_write( &log_com2, pipe.data, pipe.size );
                                        ui_led_sd_set(  true );
                                }
                                break;

                        case APP_PIPE_TAG_UART1:
                                if( write_uart1_active )
                                {
                                        ui_led_sd_set(  false );
                                        storage_write( &log_com1, pipe.data, pipe.size );
                                        ui_led_sd_set(  true );
                                }
                                break;

                        case APP_PIPE_TAG_ERROR:
                        default:
                                break;
                }
        }
}
