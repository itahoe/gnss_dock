/**
  * @file    app_task_comm.c
  * @brief   Communications Task services
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
extern  QueueHandle_t           app_que_cli_hndl;
extern  QueueHandle_t           app_que_comm_hndl;
extern  QueueHandle_t           app_que_dspl_hndl;
//static  uint8_t                 uart1_recv_data[ CFG_COMM_BLCK_SIZE_UART1_RECV_OCT ];
static  uint8_t                 uart2_recv_data[ CFG_COMM_BLCK_SIZE_UART2_RECV_OCT ];
static  uint8_t                 uart3_recv_data[ CFG_COMM_BLCK_SIZE_UART3_RECV_OCT ];
/*
static  comm_t                  uart1   =   {   .init           =   bsp_mcu_uart1_init,
                                                .xmit           =   bsp_mcu_uart1_xmit_start,
                                                .recv.open      =   bsp_mcu_uart1_recv_start,
                                                .recv.close     =   bsp_mcu_uart1_recv_stop,
                                                .recv.dma_get   =   bsp_mcu_uart1_dma_recv_ndtr_get,
                                                .recv.blck_size =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT,
                                                .recv.ndtr      =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT,
                                                .recv.data      =   uart1_recv_data,
                                                .recv.head      =   uart1_recv_data, };
*/
static  comm_t                  uart2   =   {   .init           =   bsp_mcu_uart2_init,
                                                .xmit           =   bsp_mcu_uart2_xmit_start,
                                                .recv.open      =   bsp_mcu_uart2_recv_start,
                                                .recv.close     =   bsp_mcu_uart2_recv_stop,
                                                .recv.dma_get   =   bsp_mcu_uart2_dma_recv_ndtr_get,
                                                .recv.blck_size =   CFG_COMM_BLCK_SIZE_UART2_RECV_OCT,
                                                .recv.ndtr      =   CFG_COMM_BLCK_SIZE_UART2_RECV_OCT,
                                                .recv.data      =   uart2_recv_data,
                                                .recv.head      =   uart2_recv_data, };

static  comm_t                  uart3   =   {   .init           =   bsp_mcu_uart3_init,
                                                .xmit           =   bsp_mcu_uart3_xmit_start,
                                                .recv.open      =   bsp_mcu_uart3_recv_start,
                                                .recv.close     =   bsp_mcu_uart3_recv_stop,
                                                .recv.dma_get   =   bsp_mcu_uart3_dma_recv_ndtr_get,
                                                .recv.blck_size =   CFG_COMM_BLCK_SIZE_UART3_RECV_OCT,
                                                .recv.ndtr      =   CFG_COMM_BLCK_SIZE_UART3_RECV_OCT,
                                                .recv.data      =   uart3_recv_data,
                                                .recv.head      =   uart3_recv_data, };


void app_ser2_recv_half_isr( void )
{
        app_pipe_t      pipe    =       {       .tag    =   APP_PIPE_TAG_UART2,
                                                .data   =   uart2.recv.data,
                                                .head   =   uart2.recv.head,
                                                .tile   =   uart2.recv.tile,
                                                .size   =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT  / 2};

        xQueueSendFromISR( app_que_storage_hndl, &pipe, NULL );
}


void app_ser2_recv_full_isr( void )
{
        app_pipe_t      pipe    =       {       .tag    =   APP_PIPE_TAG_UART2,
                                                .data   =   uart2.recv.data + CFG_COMM_BLCK_SIZE_UART1_RECV_OCT / 2,
                                                .head   =   uart2.recv.head,
                                                .tile   =   uart2.recv.tile,
                                                .size   =   CFG_COMM_BLCK_SIZE_UART1_RECV_OCT / 2};

        xQueueSendFromISR( app_que_storage_hndl, &pipe, NULL );
}


static
bool app_task_comm_uart_recv_hook(              comm_pipe_inp_t *       p )
{
        bool            resp            =   false;
        uint32_t        ndtr            =   p->dma_get();
        uint8_t *       head            =   p->data + (p->blck_size - ndtr);


        if( head != p->head )
        {
                if( head < p->head )
                {
                        p->size         =  (p->data + p->blck_size) - p->head;
                        head            =   p->data;
                }
                else
                {
                        p->size         =  head - p->head;
                }

                p->tile         =   p->head;
                p->head         =   head;
                p->ndtr         =   ndtr;
                resp            =   true;
        }

        return( resp );
}


void app_task_comm(                             void *                  arg )
{
        TickType_t              polling_cycle_tcks      =   CFG_COMM_POLLING_CYCLE_mSEC / portTICK_PERIOD_MS;
        bool                    no_timeout;
        bool                    recieved;
        app_pipe_t              pipe;


        (void) arg;

        //uart1.init( CFG_COMM_UART1_BAUDRATE );
        //uart1.recv.open( uart1.recv.data, uart1.recv.blck_size );

        uart2.init( CFG_COMM_UART2_BAUDRATE );
        uart2.recv.open( uart2.recv.data, uart2.recv.blck_size );

        uart3.init( CFG_COMM_UART3_BAUDRATE );
        uart3.recv.open( uart3.recv.data, uart3.recv.blck_size );


        while( true )
        {
                no_timeout      =   xQueueReceive( app_que_comm_hndl, &pipe, polling_cycle_tcks );

                if( no_timeout )
                {
                        switch( pipe.tag )
                        {
                                case APP_PIPE_TAG_UART3:
                                        uart3.xmit( pipe.data, pipe.size );
                                        APP_TRACE( "APP_COMM - APP_MSG_TYPE_SER3_XMIT\n" );
                                        break;
/*
                                case APP_PIPE_TAG_SER1_XMIT:
                                        //uart1.xmit( stream.data, stream.size );
                                        break;
*/
                                default:
                                        break;
                        }
                }
                else //queue passed by timeout
                {
/*
                        recieved        =   app_task_comm_uart_recv_hook( &uart1.recv );
                        if( recieved )
                        {
                                usb_cdc_xmit( uart1.recv.tile, uart1.recv.size );

                                stream.type     =   APP_MSG_TYPE_SER1_RECV;
                                stream.data     =   uart1.recv.tile;
                                stream.size     =   uart1.recv.size;
                                xQueueSend( app_que_dspl_hndl, &stream, NULL );
                        }
*/
                        recieved        =   app_task_comm_uart_recv_hook( &uart2.recv );
                        if( recieved )
                        {
                                uart3.xmit( uart2.recv.tile, uart2.recv.size );
                        }

                        recieved        =   app_task_comm_uart_recv_hook( &uart3.recv );
                        if( recieved )
                        {
                                uart2.xmit( uart3.recv.tile, uart3.recv.size );

                                pipe.tag        =   APP_PIPE_TAG_UART3;
                                pipe.data       =   uart3.recv.tile;
                                pipe.size       =   uart3.recv.size;
                                xQueueSend( app_que_cli_hndl, &pipe, NULL );
                        }
                }
        }
}
