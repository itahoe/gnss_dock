/**
  * @file    app_task_ui.c
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "cmsis_os.h"
#include "app.h"
#include "ui.h"
#include "pmu.h"


extern  TaskHandle_t            task_ui;
extern  TaskHandle_t            task_gnss;
extern  TaskHandle_t            task_usb;
extern  QueueHandle_t           hque_gnss_serA;
extern  QueueHandle_t           app_que_storage_hndl;


/**
  * @brief  Toggle LED1 and LED4 thread
  * @param  thread not used
  * @retval None
  */
void app_task_ui(                               void *            argument )
{

        const   TickType_t      delay_start_check_msec  =   100;
        const   TickType_t      delay_cycle_msec        =   1000 / CFG_UI_SYSTICK_HZ;

                bool            evt_ui_key_pwr;
                bool            evt_ui_key_func;
                ui_key_sts_t    key_sts;
                app_stream_t    stream;
                bool            resp;


        (void)          argument;

	ui_led_gnss_set(        UI_LED_GNSS_MODE_NONE   );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_BLACK  );
	ui_led_usb_set(         false                   );
	ui_led_sd_set(          false                   );

	ui_led_gnss_set(        UI_LED_GNSS_MODE_GPS    );
        osDelay( delay_start_check_msec );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_DGPS   );
        osDelay( delay_start_check_msec );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_RTKFLT );
        osDelay( delay_start_check_msec );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_RTKINT );
        osDelay( delay_start_check_msec );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_NONE   );

	ui_led_pwr_set(         UI_LED_RGB_COLOR_RED    );
        osDelay( delay_start_check_msec );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_GREEN  );
        osDelay( delay_start_check_msec );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_BLACK  );

	ui_led_sd_set(          true                    );
        osDelay( delay_start_check_msec );
	ui_led_sd_set(          false                   );

	ui_led_usb_set(         true                    );
        osDelay( delay_start_check_msec );


        while( true )
        {
                osDelay( delay_cycle_msec );

                evt_ui_key_pwr  =    ui_key_pwr_hook();
                evt_ui_key_func =    ui_key_func_hook();

                if( evt_ui_key_pwr )
                {
                        key_sts         =   ui_key_pwr_status();

                        switch( key_sts )
                        {
                                case UI_KEY_STS_LONG:
                                        pmu_ctl( PMU_CTL_MCU_OFF, false );
                                        break;

                                case UI_KEY_STS_SHORT:
                                        break;

                                case UI_KEY_STS_NONE:
                                case UI_KEY_STS_REPEAT:
                                case UI_KEY_STS_RELEASE:
                                default:
                                        break;
                        }
                }

                if( evt_ui_key_func )
                {
                        key_sts         =   ui_key_func_status();

                        switch( key_sts )
                        {
                                case UI_KEY_STS_LONG:
                                        stream.type     =   APP_MSG_TYPE_STORAGE_TOGGLE;

                                        do
                                        {
                                                resp    =   xQueueSend( app_que_storage_hndl, &stream, NULL );
                                        }
                                        while( resp != pdTRUE );

                                        break;

                                case UI_KEY_STS_SHORT:
                                        break;

                                case UI_KEY_STS_NONE:
                                case UI_KEY_STS_REPEAT:
                                case UI_KEY_STS_RELEASE:
                                default:
                                        break;
                        }

                }

                switch( pmu_sts_get() )
                {
                        case PMU_STS_VBUS:      ui_led_pwr_set( UI_LED_RGB_COLOR_YELLOW  ); break;
                        case PMU_STS_CHRG:      ui_led_pwr_set( UI_LED_RGB_COLOR_MAGENTA ); break;
                        case PMU_STS_BATT_FULL: ui_led_pwr_set( UI_LED_RGB_COLOR_GREEN   ); break;
                        case PMU_STS_BATT_LOW:  ui_led_pwr_set( UI_LED_RGB_COLOR_RED     ); break;
                        default:                ui_led_pwr_set( UI_LED_RGB_COLOR_WHITE   ); break;
                }
        }
}
