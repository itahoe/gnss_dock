/**
  * @file    app_task_uart3.c
  * @brief   UART3 Task services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "cmsis_os.h"
#include "bsp_mcu.h"
#include "comm.h"
#include "app.h"

#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


bool app_task_uart3_xmit(                       uint8_t *               data,
                                                size_t                  size );


extern  QueueHandle_t           app_que_uart1_hndl;
extern  QueueHandle_t           app_que_uart2_hndl;
extern  QueueHandle_t           app_que_uart3_hndl;
extern  QueueHandle_t           app_que_cli_hndl;

static  uint8_t         uart3_recv_data[ CFG_COMM_BLCK_SIZE_UART3_RECV_OCT ];

static  comm_t          uart3   =   {   .init           =   bsp_mcu_uart3_init,
                                        //.xmit           =   bsp_mcu_uart3_xmit_start,
                                        .xmit           =   app_task_uart3_xmit,
                                        .recv.open      =   bsp_mcu_uart3_recv_start,
                                        .recv.close     =   bsp_mcu_uart3_recv_stop,
                                        .recv.dma_get   =   bsp_mcu_uart3_dma_recv_ndtr_get,
                                        .recv.blck_size =   CFG_COMM_BLCK_SIZE_UART3_RECV_OCT,
                                        .recv.ndtr      =   CFG_COMM_BLCK_SIZE_UART3_RECV_OCT,
                                        .recv.data      =   uart3_recv_data,
                                        .recv.head      =   uart3_recv_data, };

static
bool app_task_uart3_xmit(                       uint8_t *               data,
                                                size_t                  size )
{
        bool            error;


        do
        {
                error   =   bsp_mcu_uart3_xmit_start( data, size );

                if( error )
                {
                        APP_TRACE( "app_task_uart3_xmit() = ERROR\n" );

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


void app_task_uart3(                            void *                  arg )
{
        TickType_t              polling_cycle_tcks      =   CFG_COMM_POLLING_CYCLE_mSEC / portTICK_PERIOD_MS;
        bool                    received;
        bool                    not_empty;
        app_pipe_t              pipe;


        (void) arg;

        uart3.init( CFG_COMM_UART3_BAUDRATE );
        uart3.recv.open( uart3.recv.data, uart3.recv.blck_size );


        while( true )
        {
                received        =   xQueueReceive( app_que_uart3_hndl, &pipe, polling_cycle_tcks );

                if( received )
                {
                        switch( pipe.tag )
                        {
                                case APP_PIPE_TAG_UART2:
                                        uart3.xmit( pipe.head, pipe.size );
                                        break;

                                case APP_PIPE_TAG_CLI:
                                        uart3.xmit( pipe.head, pipe.size );
                                        APP_TRACE( "%s xQueueReceive( app_que_uart2_hndl )\n", __FILE__ );
                                        break;

                                default:
                                        break;
                        }
                }
                //else //queue passed by timeout
                {
                        not_empty       =   comm_uart_recv_hook( &uart3.recv );

                        if( not_empty )
                        {
                                pipe.tag        =   APP_PIPE_TAG_UART3;
                                pipe.head       =   uart3.recv.tile;
                                pipe.size       =   uart3.recv.size;
                                xQueueSend( app_que_uart2_hndl,      &pipe, NULL );
                                xQueueSend( app_que_cli_hndl,        &pipe, NULL );
                        }

                }
        }
}
