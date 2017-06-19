/**
  * @file    app.h
  * @brief   main app
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32f4xx_hal.h"
#include "cmsis_os.h"


//#include "bsp_mcu.h"
#include "gnss.h"
#include "app.h"
#include "storage.h"
#include "ui.h"
#include "pmu.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


        app_t                   app;
        time_t                  time_dat        =   0;
extern  USBD_HandleTypeDef      husbd;


        QueueHandle_t           app_que_usb_cdc_hndl;
        uint8_t                 app_que_usb_cdc_alloc[  APP_QUE_SIZE_USB_CDC_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_usb_cdc;

        QueueHandle_t           app_que_cli_hndl;
        uint8_t                 app_que_cli_alloc[      APP_QUE_SIZE_CLI_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_cli;

        QueueHandle_t           app_que_storage_hndl;
        uint8_t                 app_que_storage_alloc[  APP_QUE_SIZE_STORAGE_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_storage;

        QueueHandle_t           app_que_gnss_hndl;
        uint8_t                 app_que_gnss_alloc[     APP_QUE_SIZE_GNSS_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_gnss;

        QueueHandle_t           app_que_uart1_hndl;
        uint8_t                 app_que_uart1_alloc[    APP_QUE_SIZE_UART1_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_uart1;

        QueueHandle_t           app_que_uart2_hndl;
        uint8_t                 app_que_uart2_alloc[    APP_QUE_SIZE_UART2_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_uart2;

        QueueHandle_t           app_que_uart3_hndl;
        uint8_t                 app_que_uart3_alloc[    APP_QUE_SIZE_UART3_WRDS * sizeof(app_pipe_t) ];
        StaticQueue_t           app_que_uart3;


        StackType_t             task_main_stack[        APP_TASK_STACK_SIZE_MAIN_WRDS ];
        StaticTask_t            task_main_tcb;
        TaskHandle_t            task_main;

        StackType_t             task_ui_stack[          APP_TASK_STACK_SIZE_UI_WRDS   ];
        StaticTask_t            task_ui_tcb;
        TaskHandle_t            task_ui;

        StackType_t             task_gnss_stack[        APP_TASK_STACK_SIZE_GNSS_WRDS ];
        StaticTask_t            task_gnss_tcb;
        TaskHandle_t            task_gnss;

        StackType_t             task_usb_stack[         APP_TASK_STACK_SIZE_USB_WRDS  ];
        StaticTask_t            task_usb_tcb;
        TaskHandle_t            task_usb;

        StackType_t             task_storage_stack[     APP_TASK_STACK_SIZE_STORAGE_WRDS ];
        StaticTask_t            task_storage_tcb;
        TaskHandle_t            task_storage;

        StackType_t             task_cli_stack[         APP_TASK_STACK_SIZE_CLI_WRDS ];
        StaticTask_t            task_cli_tcb;
        TaskHandle_t            task_cli;

        StackType_t             task_uart1_stack[       APP_TASK_STACK_SIZE_UART1_WRDS ];
        StaticTask_t            task_uart1_tcb;
        TaskHandle_t            task_uart1;

        StackType_t             task_uart2_stack[       APP_TASK_STACK_SIZE_UART2_WRDS ];
        StaticTask_t            task_uart2_tcb;
        TaskHandle_t            task_uart2;

        StackType_t             task_uart3_stack[       APP_TASK_STACK_SIZE_UART3_WRDS ];
        StaticTask_t            task_uart3_tcb;
        TaskHandle_t            task_uart3;

void app_ser1_recv_idle_isr(                    uint32_t                cnt )
{
}

void app_ser1_xmit_full_isr( void )
{
	//Initiate next USB packet transfer once UART completes transfer (transmitting data over Tx line)
	USBD_CDC_ReceivePacket( &husbd );
}

void app_ser2_recv_idle_isr(                    uint32_t                cnt )
{
}

void app_ser2_xmit_full_isr( void )
{
}

void app_ser3_recv_idle_isr(                    uint32_t                cnt )
{
}

void app_ser3_recv_half_isr( void )
{
}

void app_ser3_recv_full_isr( void )
{
}

void app_ser3_xmit_full_isr( void )
{
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main( void )
{
        int             task_main_parameter             =  1;
        int             task_ui_parameter               =  1;
        int             task_gnss_parameter             =  1;
        int             task_usb_parameter              =  1;
        int             task_storage_parameter          =  1;
        int             task_cli_parameter              =  1;
        int             task_uart1_parameter            =  1;
        int             task_uart2_parameter            =  1;
        int             task_uart3_parameter            =  1;


	#if defined( NDEBUG )
	//skip debugger setup
	#else
	HAL_DBGMCU_EnableDBGSleepMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGStandbyMode();
	#endif

        app_clk_init();

        app_cfg_init( &app.cfg );

        APP_TRACE( "GNSS_DOCK Start\n" );

        HAL_Init();
	ui_init();
        ui_led_pwr_set( UI_LED_RGB_COLOR_BLACK );
	ui_led_gnss_set( UI_LED_GNSS_MODE_NONE );
	//ui_led_gnss_set( UI_LED_RGB_COLOR_BLACK );
	ui_led_sd_set( false );
	ui_led_usb_set( false );

	pmu_init();

	HAL_Delay( CFG_UI_KEY_LONG_MAX_mSEC );

	if( !ui_key_pwr_forced() )
	{
		pmu_ctl( PMU_CTL_MCU_OFF, false );
	}

	HAL_Delay( CFG_UI_LED_FLSH_LONG_mSEC );

        ui_key_pwr_reset();

        pmu_ctl( PMU_CTL_GNSS_LDO_ON, true );


        app_que_usb_cdc_hndl    =   xQueueCreateStatic( APP_QUE_SIZE_USB_CDC_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_usb_cdc_alloc,
                                                        &app_que_usb_cdc );

        app_que_cli_hndl        =   xQueueCreateStatic( APP_QUE_SIZE_CLI_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_cli_alloc,
                                                        &app_que_cli );

        app_que_storage_hndl    =   xQueueCreateStatic( APP_QUE_SIZE_STORAGE_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_storage_alloc,
                                                        &app_que_storage );

        app_que_gnss_hndl       =   xQueueCreateStatic( APP_QUE_SIZE_GNSS_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_gnss_alloc,
                                                        &app_que_gnss );

        app_que_uart1_hndl      =   xQueueCreateStatic( APP_QUE_SIZE_UART1_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_uart1_alloc,
                                                        &app_que_uart1 );

        app_que_uart2_hndl      =   xQueueCreateStatic( APP_QUE_SIZE_UART2_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_uart2_alloc,
                                                        &app_que_uart2 );

        app_que_uart3_hndl      =   xQueueCreateStatic( APP_QUE_SIZE_UART3_WRDS,
                                                        sizeof(app_pipe_t),
                                                        app_que_uart3_alloc,
                                                        &app_que_uart3 );


        task_main               =   xTaskCreateStatic(  app_task_main,
                                                        "MAIN",
                                                        APP_TASK_STACK_SIZE_MAIN_WRDS,
                                                        (void *) task_main_parameter,
                                                        osPriorityNormal,
                                                        task_main_stack,
                                                        &task_main_tcb );

        task_ui                 =   xTaskCreateStatic(  app_task_ui,
                                                        "UI",
                                                        APP_TASK_STACK_SIZE_UI_WRDS,
                                                        (void *) task_ui_parameter,
                                                        osPriorityNormal,
                                                        task_ui_stack,
                                                        &task_ui_tcb );

        task_gnss               =   xTaskCreateStatic(  app_task_gnss,
                                                        "GNSS",
                                                        APP_TASK_STACK_SIZE_GNSS_WRDS,
                                                        (void *) task_gnss_parameter,
                                                        osPriorityNormal,
                                                        task_gnss_stack,
                                                        &task_gnss_tcb );

        task_usb                =   xTaskCreateStatic(  app_task_usb,
                                                        "USB",
                                                        APP_TASK_STACK_SIZE_USB_WRDS,
                                                        (void *) task_usb_parameter,
                                                        osPriorityNormal,
                                                        task_usb_stack,
                                                        &task_usb_tcb );

        task_storage            =   xTaskCreateStatic(  app_task_storage,
                                                        "STORAGE",
                                                        APP_TASK_STACK_SIZE_STORAGE_WRDS,
                                                        (void *) task_storage_parameter,
                                                        osPriorityNormal,
                                                        task_storage_stack,
                                                        &task_storage_tcb );

        task_cli                =   xTaskCreateStatic(  app_task_cli,
                                                        "CLI",
                                                        APP_TASK_STACK_SIZE_CLI_WRDS,
                                                        (void *) task_cli_parameter,
                                                        osPriorityNormal,
                                                        task_cli_stack,
                                                        &task_cli_tcb );

        task_uart1              =   xTaskCreateStatic(  app_task_uart1,
                                                        "UART1",
                                                        APP_TASK_STACK_SIZE_UART1_WRDS,
                                                        (void *) task_uart1_parameter,
                                                        osPriorityNormal,
                                                        task_uart1_stack,
                                                        &task_uart1_tcb );

        task_uart2              =   xTaskCreateStatic(  app_task_uart2,
                                                        "UART2",
                                                        APP_TASK_STACK_SIZE_UART2_WRDS,
                                                        (void *) task_uart2_parameter,
                                                        osPriorityNormal,
                                                        task_uart2_stack,
                                                        &task_uart2_tcb );

        task_uart3              =   xTaskCreateStatic(  app_task_uart3,
                                                        "UART3",
                                                        APP_TASK_STACK_SIZE_UART3_WRDS,
                                                        (void *) task_uart3_parameter,
                                                        osPriorityNormal,
                                                        task_uart3_stack,
                                                        &task_uart3_tcb );


        osKernelStart();

        #if defined( NDEBUG )
        NVIC_SystemReset();
        #else
        while( true );
	#endif
}
