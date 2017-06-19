/**
  * @file    app_task_uart1.c
  * @brief   UART1 Task services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "cmsis_os.h"
#include "bsp_mcu.h"
#include "comm.h"
#include "app.h"

#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


extern  QueueHandle_t           app_que_usb_cdc_hndl;
extern  QueueHandle_t           app_que_storage_hndl;
extern  QueueHandle_t           app_que_uart1_hndl;
extern  QueueHandle_t           app_que_gnss_hndl;
//extern  QueueHandle_t           app_que_cli_hndl;

static  uint8_t         uart1_recv_data[ CFG_COMM_BLCK_SIZE_UART1_RECV_OCT ];

static  comm_t          uart1   =   {   .init           =   bsp_mcu_uart1_init,
                                        .xmit           =   bsp_mcu_uart1_xmit_start,
                                        .recv.open      =   bsp_mcu_uart1_recv_start,
                                        .recv.close     =   bsp_mcu_uart1_recv_stop,
                                        .recv.dma_get   =   bsp_mcu_uart1_dma_recv_ndtr_get,
                                        .recv.blck_size =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT,
                                        .recv.ndtr      =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT,
                                        .recv.data      =   uart1_recv_data,
                                        .recv.head      =   uart1_recv_data, };


void app_ser1_recv_half_isr( void )
{
        app_pipe_t      pipe    =   {   .tag            =   APP_PIPE_TAG_UART1,
                                        //.data           =   uart1.recv.data,
                                        //.tile           =   uart1.recv.tile,
                                        //.head           =   uart1.recv.head,
                                        .head           =   uart1.recv.data,
                                        .size           =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &pipe, NULL );
}


void app_ser1_recv_full_isr( void )
{
        app_pipe_t      pipe    =   {   .tag            =   APP_PIPE_TAG_UART1,
                                        //.data           =   uart1.recv.data + CFG_COMM_BLCK_SIZE_UART1_RECV_OCT/ 2,
                                        //.tile           =   uart1.recv.tile,
                                        //.head           =   uart1.recv.head,
                                        .head           =   uart1.recv.data + CFG_COMM_BLCK_SIZE_UART1_RECV_OCT/ 2,
                                        .size           =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &pipe, NULL );
}


static
bool app_task_uart1_xmit(                       uint8_t *               data,
                                                size_t                  size )
{
        bool            error;


        do
        {
                error   =   bsp_mcu_uart1_xmit_start( data, size );

                if( error )
                {
                        APP_TRACE( "app_task_uart1_xmit() = ERROR\n" );

                        if( size == 0 )
                        {
                                break;
                        }

                        taskYIELD();
                }
        }
        while( error );

        return( error );
}


void app_task_uart1(                            void *                  arg )
{
        TickType_t              polling_cycle_tcks      =   CFG_COMM_POLLING_CYCLE_mSEC / portTICK_PERIOD_MS;
        bool                    received;
        bool                    not_empty;
        app_pipe_t              pipe;


        (void) arg;

        uart1.init( CFG_COMM_UART1_BAUDRATE );
        uart1.recv.open( uart1.recv.data, uart1.recv.blck_size );


        while( true )
        {
                received        =   xQueueReceive( app_que_uart1_hndl, &pipe, polling_cycle_tcks );

                if( received )
                {
                        switch( pipe.tag )
                        {
                                case APP_PIPE_TAG_USB_RECV:
                                        //uart1.xmit( pipe.head, pipe.size );
                                        app_task_uart1_xmit( pipe.head, pipe.size );
                                        break;

                                default:
                                        break;
                        }
                }
                else //queue passed by timeout
                {
                        not_empty       =   comm_uart_recv_hook( &uart1.recv );

                        if( not_empty )
                        {
                                pipe.tag        =   APP_PIPE_TAG_UART1;
                                pipe.head       =   uart1.recv.tile;
                                pipe.size       =   uart1.recv.size;
                                xQueueSend( app_que_usb_cdc_hndl,   &pipe, NULL );
                                xQueueSend( app_que_gnss_hndl,      &pipe, NULL );
                                //xQueueSend( app_que_cli_hndl,       &pipe, NULL );
                        }
                }
        }
}
