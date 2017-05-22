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


#define APP_QUE_SIZE_USB_CDC_WRDS               8
#define APP_QUE_SIZE_CLI_WRDS                   8
#define APP_QUE_SIZE_STORAGE_WRDS               8
#define APP_QUE_SIZE_GNSS_WRDS                  8
#define APP_QUE_SIZE_COMM_WRDS                  8
#define APP_QUE_SIZE_DSPL_WRDS                  8
#define APP_QUE_SIZE_UART1_WRDS                 8
#define APP_QUE_SIZE_UART2_WRDS                 8
#define APP_QUE_SIZE_UART3_WRDS                 8

#define APP_TASK_STACK_SIZE_MAIN_WRDS           configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_UI_WRDS             configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_GNSS_WRDS           configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_DSPL_WRDS           configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_USB_WRDS            400
#define APP_TASK_STACK_SIZE_STORAGE_WRDS        400
#define APP_TASK_STACK_SIZE_CLI_WRDS            configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_COMM_WRDS           400
#define APP_TASK_STACK_SIZE_UART1_WRDS          configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_UART2_WRDS          configMINIMAL_STACK_SIZE
#define APP_TASK_STACK_SIZE_UART3_WRDS          configMINIMAL_STACK_SIZE


typedef enum    app_gnss_data_type_e
{
        APP_GNSS_DATA_TYPE_STREAM,
        APP_GNSS_DATA_TYPE_LOCAL,
} app_gnss_data_type_t;

typedef enum    app_pipe_tag_e
{
        APP_PIPE_TAG_ERROR,
        APP_PIPE_TAG_UART1,
        APP_PIPE_TAG_UART2,
        APP_PIPE_TAG_UART3,
        APP_PIPE_TAG_USB_RECV,
        APP_PIPE_TAG_CLI,
        APP_PIPE_TAG_STORAGE_TOGGLE,
} app_pipe_tag_t;

typedef	struct	app_cfg_s
{
	size_t          log_mode;
} app_cfg_t;

typedef	struct	app_pipe_s
{
        app_pipe_tag_t          tag;
        uint8_t *               data;
        size_t                  size;
        uint8_t *               head;
        uint8_t *               tile;
} app_pipe_t;

typedef	struct	app_s
{
	size_t          tick_1hz;
	size_t          cdc_tick;
        app_cfg_t       cfg;

	#ifndef NDEBUG
	size_t          ism_pckts_sent;
	size_t          tick_1hz_cnt;
	#endif

}	app_t;


void app_error( void );
HAL_StatusTypeDef app_clk_init( void );


bool app_cfg_init(                              app_cfg_t *             p );

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
void app_task_uart1(                            void *                  arg );
void app_task_uart2(                            void *                  arg );
void app_task_uart3(                            void *                  arg );

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


#endif	//APP_H
