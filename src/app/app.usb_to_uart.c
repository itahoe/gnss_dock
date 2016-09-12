/**
  * @file    app.usb_to_uart.c
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <time.h>
#include "ui.h"
#include "bsp_mcu.h"
#include "fifo.h"
#include "app.h"
#include "pmu.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"


static  app_t                   app;
        time_t                  time_dat        =   0;
        USBD_HandleTypeDef      husbd;
        TIM_HandleTypeDef       htim_cdc;

static  uint8_t                 data_uart1_tx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
static  uint8_t                 data_uart1_rx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
//static  uint8_t                 data_uart2_tx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
static  uint8_t                 data_uart2_rx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
//static  uint8_t                 data_uart3_tx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
static  uint8_t                 data_uart3_rx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];

        fifo_t                  fifo_uart1_tx   =   {   .data     =   data_uart1_tx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };

        fifo_t                  fifo_uart1_rx   =   {   .data     =   data_uart1_rx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };

/*
        gnss_fifo_t             fifo_uart2_tx   =   {   .data     =   data_uart2_tx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };
*/
        fifo_t                  fifo_uart2_rx   =   {   .data     =   data_uart2_rx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };

/*
        gnss_fifo_t             fifo_uart3_tx   =   {   .data     =   data_uart3_tx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };
*/
        fifo_t                  fifo_uart3_rx   =   {   .data     =   data_uart3_rx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };


/**
  * @brief  This function is executed in case of error occurrence.
  */
void	app_error( void )
{
	uint32_t        delay_msec  =   1000;

	while( true )
	{
		ui_led_sd_toggle();
		ui_led_usb_toggle();
		HAL_Delay( delay_msec );
	}
}

/**
  * @brief  Switch the PLL source from HSI to HSE bypass, and select the PLL as SYSCLK
  */
static
void app_clock_config( void )
{
	RCC_ClkInitTypeDef              clk     =   {0};
	RCC_OscInitTypeDef              osc     =   {0};
	RCC_PeriphCLKInitTypeDef        PeriphClkInitStruct;


	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG( PWR_REGULATOR_VOLTAGE_SCALE2 );

	osc.OscillatorType      =   RCC_OSCILLATORTYPE_HSE;
	osc.HSEState            =   RCC_HSE_ON;
	osc.PLL.PLLSource       =   RCC_PLLSOURCE_HSE;

	osc.HSICalibrationValue =   0x10;
	osc.PLL.PLLState        =   RCC_PLL_ON;
	osc.PLL.PLLM            =   16;
	osc.PLL.PLLN            =   200;
	osc.PLL.PLLP            =   RCC_PLLP_DIV2;
	osc.PLL.PLLQ            =   15;

	if( HAL_RCC_OscConfig( &osc ) != HAL_OK )
	{
		app_error();
	}

	clk.ClockType           =   (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clk.SYSCLKSource        =   RCC_SYSCLKSOURCE_PLLCLK;
	clk.AHBCLKDivider       =   RCC_SYSCLK_DIV1;
	clk.APB1CLKDivider      =   RCC_HCLK_DIV2;
	clk.APB2CLKDivider      =   RCC_HCLK_DIV1;

	if( HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_3 ) != HAL_OK )
	{
		app_error();
	}

	/* Activate the OverDrive to reach the 180 MHz Frequency */  
	HAL_PWREx_EnableOverDrive();

	/* Select PLLSAI output as USB clock source */
	PeriphClkInitStruct.PLLSAI.PLLSAIM          =   16;
	PeriphClkInitStruct.PLLSAI.PLLSAIN          =   384;
	PeriphClkInitStruct.PLLSAI.PLLSAIP          =   RCC_PLLSAIP_DIV8;
	PeriphClkInitStruct.PeriphClockSelection    =   RCC_PERIPHCLK_CK48;
	PeriphClkInitStruct.Clk48ClockSelection     =   RCC_CK48CLKSOURCE_PLLSAIP;
	HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct );

	SystemCoreClockUpdate();
}

/**
 * @brief
 */
void HAL_UART_RxHalfCpltCallback(       UART_HandleTypeDef *    huart )
{
        if(      huart->Instance == USART1 )
        {
                //gnss_read( &gnss, data, size );
        }
        else if( huart->Instance == USART2 )
        {
                //bsp_mcu_uart3_xmit_start( data_uart2_rx[0], CFG_GNSS_BLCK_SIZE_OCT/2 );
        }
        else if( huart->Instance == USART3 )
        {
                //bsp_mcu_uart2_xmit_start( data_uart3_rx[0], CFG_GNSS_BLCK_SIZE_OCT/2 );
                usb_cdc_send( data_uart3_rx[0], CFG_GNSS_BLCK_SIZE_OCT/2 );

        }
}

/**
 * @brief
 */
void HAL_UART_RxCpltCallback(           UART_HandleTypeDef *    huart )
{
        if( huart->Instance == USART3 )
        {
                usb_cdc_send( data_uart3_rx[1], CFG_GNSS_BLCK_SIZE_OCT/2 );
        }
}

/**
 * @brief
 */
void HAL_TIM_PeriodElapsedCallback(     TIM_HandleTypeDef *     htim_cdc )
{
        //gnss_uart_rx_hook( &fifo_uart1_rx );
        //app_uart2_rx_hook( &fifo_uart2_rx );
        //app_uart3_rx_hook( &fifo_uart3_rx );

        fifo_t *                p       =   &fifo_uart3_rx;
        size_t                  tile    =   p->tile;
        size_t                  size    =   bsp_mcu_uart1_recv_size_get();
        size_t                  head    =   CFG_GNSS_BLCK_SIZE_OCT - size;


	if( head > tile )
	{
		size            =   head - tile;
		p->tile         +=  usb_cdc_send( p->data + tile, size );
	}
	else if( head < tile )
	{
		if( p->overcome )
		{
			p->overcome     = false;
			size            =   CFG_GNSS_BLCK_SIZE_OCT - tile;
			usb_cdc_send( p->data + tile, size );
	                p->tile         =   0;

			#ifndef  NDEBUG
			p->total_overcomes++;
			#endif //NDEBUG
		}
		else
		{
			#ifndef  NDEBUG
			p->total_overruns++;
			#endif //NDEBUG
		}
	}

        #ifndef  NDEBUG
        p->total_data           +=   size;
        #endif //NDEBUG

}

/**
 * @brief
 */
void app_systick_hook( void )
{
	if( ++(app.tick_1hz) > BSP_SYSTICK_HZ )
	{
		app.tick_1hz        =    0;
		app.evt.tick_1hz    =    true;

		time_dat++;

		#ifndef	NDEBUG
		app.tick_1hz_cnt++;
		#endif
	}
}

/**
 * @brief App entry
 */
int main( void )
{
	app_clock_config();
	SysTick_Config(SystemCoreClock / BSP_SYSTICK_HZ);
	HAL_Init();

	ui_init();
	ui_led_sd_set(          false                   );
	ui_led_usb_set(         false                   );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_NONE   );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_BLACK  );

	pmu_init();
	pmu_ctl( PMU_CTL_GNSS_LDO_ON, false );

	__enable_irq();

	HAL_Delay( UI_KEY_LONG_TCKS );

	ui_led_sd_flash(        UI_LED_FLSH_LONG_TCKS );
	ui_led_usb_flash(       UI_LED_FLSH_LONG_TCKS );
	ui_led_gnss_flash(      UI_LED_FLSH_LONG_TCKS );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_WHITE );

	HAL_Delay( CFG_UI_LED_FLSH_LONG_mSEC );

	pmu_ctl( PMU_CTL_GNSS_LDO_ON, true );

	USBD_Init(                      &husbd, &VCP_Desc,      0);
	USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
	USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
	USBD_Start( &husbd );

        bsp_mcu_uart2_init( 115200 );
        bsp_mcu_uart3_init( 9600 );
        //bsp_mcu_uart2_xmit_start( data_uart2_tx[0], CFG_GNSS_BLCK_SIZE_OCT );
        bsp_mcu_uart2_recv_start( data_uart2_rx[0], CFG_GNSS_BLCK_SIZE_OCT );
        //bsp_mcu_uart3_xmit_start( data_uart3_tx[0], CFG_GNSS_BLCK_SIZE_OCT );
        bsp_mcu_uart3_recv_start( data_uart3_rx[0], CFG_GNSS_BLCK_SIZE_OCT );


	while( true )
	{
		if( app.evt.tick_1hz )
		{
			app.evt.tick_1hz    =   false;

			switch( pmu_sts_get() )
			{
				case PMU_STS_VBUS:      ui_led_pwr_set( UI_LED_RGB_COLOR_YELLOW  ); break;
				case PMU_STS_CHRG:      ui_led_pwr_set( UI_LED_RGB_COLOR_BLUE    ); break;
				case PMU_STS_BATT_FULL: ui_led_pwr_set( UI_LED_RGB_COLOR_GREEN   ); break;
				case PMU_STS_BATT_LOW:  ui_led_pwr_set( UI_LED_RGB_COLOR_RED     ); break;
				default:                ui_led_pwr_set( UI_LED_RGB_COLOR_WHITE   ); break;
			}

			ui_led_pwr_flash( UI_LED_FLSH_SHRT_TCKS );

			//APP_TRACE(	"\n" );
			//APP_TRACE(	"FLOG=%c ", flog.sts.enable ? '1' : '0' );
			//APP_TRACE(	"OVR=%d ", gnss_data_uart_rx.total_overruns );
			//APP_TRACE(	"OVC=%d ", gnss_data_uart_rx.total_overcomes );
			//APP_TRACE(	"DAT=%d ", gnss_data_uart_rx.total_data );
		}

	}
}
