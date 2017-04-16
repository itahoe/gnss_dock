/**
  * @file    app.h
  * @brief   main app header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include "app_trace.h"
#include "config.h"


#define APP_QUE_SIZE_USB_CDC_WRDS               4
#define APP_QUE_SIZE_CLI_WRDS                   4
#define APP_QUE_SIZE_STORAGE_WRDS               4
#define APP_QUE_SIZE_GNSS_WRDS                  4
#define APP_QUE_SIZE_COMM_WRDS                  4

#define APP_TASK_STACK_SIZE_MAIN_WRDS           configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_UI_WRDS             configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_GNSS_WRDS           configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_DSPL_WRDS           configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_USB_WRDS            400
#define APP_TASK_STACK_SIZE_STORAGE_WRDS        400
#define APP_TASK_STACK_SIZE_CLI_WRDS            configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_COMM_WRDS           400

typedef enum    app_gnss_data_type_e
{
        APP_GNSS_DATA_TYPE_STREAM,
        APP_GNSS_DATA_TYPE_LOCAL,
} app_gnss_data_type_t;

typedef enum    app_msg_type_e
{
        APP_MSG_TYPE_ERROR,
        APP_MSG_TYPE_SER1_RECV,
        APP_MSG_TYPE_SER2_RECV,
        APP_MSG_TYPE_SER3_RECV,
        APP_MSG_TYPE_USB_RECV,
        APP_MSG_TYPE_CLI_XMIT,
        APP_MSG_TYPE_STORAGE_TOGGLE,
} app_msg_type_t;

#pragma pack(4)
typedef	struct	app_fifo_s
{
	uint8_t *               head;
	uint8_t *               tile;
	uint8_t                 data[ CFG_GNSS_BLCK_SIZE_OCT ];
} app_fifo_t;
#pragma pack()

typedef	struct	app_cfg_s
{
	size_t          log_mode;
} app_cfg_t;

#pragma pack(4)
typedef	struct	app_stream_s
{
        app_msg_type_t          type;
        uint8_t *               data;
        uint8_t *               head;
        uint8_t *               tile;
        size_t                  size;
} app_stream_t;
#pragma pack()

typedef	struct	app_s
{
	size_t          tick_1hz;
	size_t          cdc_tick;
        app_cfg_t       cfg;

	#ifndef NDEBUG
	size_t          ism_pckts_sent;
	size_t          tick_1hz_cnt;

        volatile        int           irq_cnt_uart1;
        volatile        int           irq_cnt_uart1_dma_rx;
        volatile        int           irq_cnt_uart1_dma_tx;

        volatile        int           irq_cnt_uart2;
        volatile        int           irq_cnt_uart2_dma_rx;
        volatile        int           irq_cnt_uart2_dma_tx;

        volatile        int           irq_cnt_uart3;
        volatile        int           irq_cnt_uart3_dma_rx;
        volatile        int           irq_cnt_uart3_dma_tx;
	#endif

}	app_t;


void app_error( void );
HAL_StatusTypeDef app_clk_init( void );


bool app_cfg_init( void );

uint32_t        app_cfg_read(           const   uint32_t                addr );

void            app_cfg_write(          const   uint32_t                addr,
                                                uint32_t                data );

void app_systick_hook( void );

void app_task_main(                             void *                  arg );
void app_task_ui(                               void *                  arg );
void app_task_gnss(                             void *                  arg );
void app_task_dspl(                             void *                  arg );
void app_task_usb(                              void *                  arg );
void app_task_storage(                          void *                  arg );
void app_task_cli(                              void *                  arg );
void app_task_comm(                             void *                  arg );

void app_ser1_recv_idle_isr(                    uint32_t        cnt     );
void app_ser1_recv_half_isr(                    void                    );
void app_ser1_recv_full_isr(                    void                    );
void app_ser1_xmit_full_isr(                    void                    );

void app_ser2_recv_idle_isr(                    uint32_t        cnt     );
void app_ser2_recv_half_isr(                    void                    );
void app_ser2_recv_full_isr(                    void                    );
void app_ser2_xmit_full_isr(                    void                    );

void app_ser3_recv_idle_isr(                    uint32_t        cnt     );
void app_ser3_recv_half_isr(                    void                    );
void app_ser3_recv_full_isr(                    void                    );
void app_ser3_xmit_full_isr(                    void                    );


void app_irq_cnt_uart3( void );
void app_irq_cnt_uart3_dma_rx( void );
void app_irq_cnt_uart3_dma_tx( void );

void app_irq_cnt_uart2( void );
void app_irq_cnt_uart2_dma_rx( void );
void app_irq_cnt_uart2_dma_tx( void );

void app_irq_cnt_uart1( void );
void app_irq_cnt_uart1_dma_rx( void );
void app_irq_cnt_uart1_dma_tx( void );


#endif	//APP_H
