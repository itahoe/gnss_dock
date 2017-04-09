/**
  * @file    app.h
  * @brief   main app
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "stm32f4xx_hal.h"
#include "cmsis_os.h"


#include "bsp_mcu.h"
#include "gnss.h"
#include "app.h"
#include "storage.h"
#include "ui.h"
#include "pmu.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


        app_t                   app;
        app_fifo_t              ser1_recv;
        app_fifo_t              ser2_recv;
        app_fifo_t              ser3_recv;
        time_t                  time_dat        =   0;
extern  USBD_HandleTypeDef      husbd;


        QueueHandle_t           app_que_usb_cdc_hndl;
        uint8_t                 app_que_usb_cdc_alloc[  APP_QUE_SIZE_USB_CDC_WRDS * sizeof( app_stream_t ) ];
        StaticQueue_t           app_que_usb_cdc;

        QueueHandle_t           app_que_cli_hndl;
        uint8_t                 app_que_cli_alloc[      APP_QUE_SIZE_CLI_WRDS * sizeof( app_stream_t ) ];
        StaticQueue_t           app_que_cli;

        QueueHandle_t           app_que_storage_hndl;
        uint8_t                 app_que_storage_alloc[  APP_QUE_SIZE_STORAGE_WRDS * sizeof( app_stream_t ) ];
        StaticQueue_t           app_que_storage;

        QueueHandle_t           app_que_gnss_hndl;
        uint8_t                 app_que_gnss_alloc[     APP_QUE_SIZE_GNSS_WRDS * sizeof( app_stream_t ) ];
        StaticQueue_t           app_que_gnss;


        StackType_t             task_main_stack[        APP_TASK_STACK_SIZE_MAIN_WRDS ];
        StaticTask_t            task_main_tcb;
        TaskHandle_t            task_main;

        StackType_t             task_ui_stack[          APP_TASK_STACK_SIZE_UI_WRDS   ];
        StaticTask_t            task_ui_tcb;
        TaskHandle_t            task_ui;

        StackType_t             task_gnss_stack[        APP_TASK_STACK_SIZE_GNSS_WRDS ];
        StaticTask_t            task_gnss_tcb;
        TaskHandle_t            task_gnss;

        StackType_t             task_dspl_stack[        APP_TASK_STACK_SIZE_DSPL_WRDS  ];
        StaticTask_t            task_dspl_tcb;
        TaskHandle_t            task_dspl;

        StackType_t             task_usb_stack[         APP_TASK_STACK_SIZE_USB_WRDS  ];
        StaticTask_t            task_usb_tcb;
        TaskHandle_t            task_usb;

        StackType_t             task_storage_stack[     APP_TASK_STACK_SIZE_STORAGE_WRDS ];
        StaticTask_t            task_storage_tcb;
        TaskHandle_t            task_storage;

        StackType_t             task_cli_stack[         APP_TASK_STACK_SIZE_CLI_WRDS ];
        StaticTask_t            task_cli_tcb;
        TaskHandle_t            task_cli;

void app_irq_cnt_uart3( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart3++;
        #endif
}

void app_irq_cnt_uart2( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart2++;
        #endif
}

void app_irq_cnt_uart1( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart1++;
        #endif
}


void app_irq_cnt_uart3_dma_rx( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart3_dma_rx++;
        #endif
}

void app_irq_cnt_uart2_dma_rx( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart2_dma_rx++;
        #endif
}

void app_irq_cnt_uart1_dma_rx( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart1_dma_rx++;
        #endif
}


void app_irq_cnt_uart3_dma_tx( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart3_dma_tx++;
        #endif
}

void app_irq_cnt_uart2_dma_tx( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart2_dma_tx++;
        #endif
}

void app_irq_cnt_uart1_dma_tx( void )
{
	#ifndef NDEBUG
        app.irq_cnt_uart1_dma_tx++;
        #endif
}




void app_ser1_recv_idle_isr(                    uint32_t                cnt )
{
        bool            resp;


        ser1_recv.head          =  (ser1_recv.data + CFG_GNSS_BLCK_SIZE_OCT) - cnt;

        xTaskNotifyFromISR(     task_dspl,
                                APP_GNSS_DATA_TYPE_STREAM,
                                eSetValueWithOverwrite,
                                pdFALSE );

        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER1_RECV,
                                                .data   =   ser1_recv.data,
                                                .head   =   ser1_recv.head,
                                                .tile   =   ser1_recv.tile,
                                                .size   =   cnt };

        resp    =   xQueueSendFromISR( app_que_usb_cdc_hndl, &stream, NULL );

        if( resp != pdTRUE )
        {
                //APP_TRACE( "app_ser1_recv_idle_isr() :: xQueueSendFromISR( app_que_usb_cdc_hndl ) == != pdTRUE\n" );
        }
/*
        resp            =   xQueueSendFromISR( app_que_gnss_hndl, &stream, NULL );

        if( resp != pdTRUE )
        {
                APP_TRACE( "app_ser1_recv_idle_isr() :: xQueueSendFromISR( app_que_gnss_hndl ) == != pdTRUE\n" );
        }
*/
        ser1_recv.tile  =   ser1_recv.head;
}


void app_ser1_recv_half_isr( void )
{
        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER1_RECV,
                                                .data   =   ser1_recv.data,
                                                .head   =   ser1_recv.head,
                                                .tile   =   ser1_recv.tile,
                                                .size   =   CFG_GNSS_BLCK_SIZE_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &stream, NULL );
}


void app_ser1_recv_full_isr( void )
{
        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER1_RECV,
                                                .data   =   ser1_recv.data + CFG_GNSS_BLCK_SIZE_OCT / 2,
                                                .head   =   ser1_recv.head,
                                                .tile   =   ser1_recv.tile,
                                                .size   =   CFG_GNSS_BLCK_SIZE_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &stream, NULL );
}


void app_ser1_xmit_full_isr( void )
{
	//Initiate next USB packet transfer once UART completes transfer (transmitting data over Tx line)
	USBD_CDC_ReceivePacket( &husbd );
}


void app_ser2_recv_half_isr( void )
{
        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER2_RECV,
                                                .data   =   ser2_recv.data,
                                                .head   =   ser2_recv.head,
                                                .tile   =   ser2_recv.tile,
                                                .size   =   CFG_GNSS_BLCK_SIZE_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &stream, NULL );
}


void app_ser2_recv_full_isr( void )
{
        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER2_RECV,
                                                .data   =   ser2_recv.data + CFG_GNSS_BLCK_SIZE_OCT / 2,
                                                .head   =   ser2_recv.head,
                                                .tile   =   ser2_recv.tile,
                                                .size   =   CFG_GNSS_BLCK_SIZE_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &stream, NULL );
}


void app_ser2_recv_idle_isr(                    uint32_t                cnt )
{
        bool            resp;


        ser2_recv.head  =  (ser2_recv.data + CFG_GNSS_BLCK_SIZE_OCT) - cnt;

        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER2_RECV,
                                                .data   =   ser2_recv.data,
                                                .head   =   ser2_recv.head,
                                                .tile   =   ser2_recv.tile,
                                                .size   =   cnt };

        resp            =   xQueueSendFromISR( app_que_gnss_hndl, &stream, NULL );

        if( resp != pdTRUE )
        {
                //queue send error
        }

        ser2_recv.tile  =   ser2_recv.head;
}


void app_ser2_xmit_full_isr( void )
{
}


void app_ser3_recv_idle_isr(                    uint32_t                cnt )
{
        bool            resp;


        ser3_recv.head  =  (ser3_recv.data + CFG_GNSS_BLCK_SIZE_OCT) - cnt;

        app_stream_t    stream  =       {       .type   =   APP_MSG_TYPE_SER3_RECV,
                                                .data   =   ser3_recv.data,
                                                .head   =   ser3_recv.head,
                                                .tile   =   ser3_recv.tile,
                                                .size   =   cnt };

        resp            =   xQueueSendFromISR( app_que_gnss_hndl, &stream, NULL );

        if( resp != pdTRUE )
        {
                APP_TRACE( "app_ser3_recv_idle_isr() :: xQueueSendFromISR( app_que_gnss_hndl) == != pdTRUE\n" );
        }

        ser3_recv.tile  =   ser3_recv.head;
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
        int             task_dspl_parameter             =  1;
        int             task_usb_parameter              =  1;
        int             task_storage_parameter          =  1;
        int             task_cli_parameter              =  1;


	#if defined( NDEBUG )
	//skip debugger setup
	#else
	HAL_DBGMCU_EnableDBGSleepMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGStandbyMode();
	#endif

        app_clk_init();

        app_cfg_init();
        app.cfg.log_mode        =   app_cfg_read( RTC_BKP_DR0 );

        APP_TRACE( "GNSS_DOCK Start\n" );

        HAL_Init();
	ui_init();
        ui_led_pwr_set( UI_LED_RGB_COLOR_BLACK );

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
                                                        sizeof( app_stream_t ),
                                                        app_que_usb_cdc_alloc,
                                                        &app_que_usb_cdc );

        app_que_cli_hndl        =   xQueueCreateStatic( APP_QUE_SIZE_CLI_WRDS,
                                                        sizeof( app_stream_t ),
                                                        app_que_cli_alloc,
                                                        &app_que_cli );

        app_que_storage_hndl    =   xQueueCreateStatic( APP_QUE_SIZE_STORAGE_WRDS,
                                                        sizeof( app_stream_t ),
                                                        app_que_storage_alloc,
                                                        &app_que_storage );

        app_que_gnss_hndl       =   xQueueCreateStatic( APP_QUE_SIZE_GNSS_WRDS,
                                                        sizeof( app_stream_t ),
                                                        app_que_gnss_alloc,
                                                        &app_que_gnss );


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

        task_dspl               =   xTaskCreateStatic(  app_task_dspl,
                                                        "DSPL",
                                                        APP_TASK_STACK_SIZE_DSPL_WRDS,
                                                        (void *) task_dspl_parameter,
                                                        osPriorityNormal,
                                                        task_dspl_stack,
                                                        &task_dspl_tcb );

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

        osKernelStart();

        #if defined( NDEBUG )
        NVIC_SystemReset();
        #else
        while( true );
	#endif
}
