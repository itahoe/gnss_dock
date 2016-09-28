/**
  * @file    app.c
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "ui.h"
#include "bsp_mcu.h"
#include "gnss.h"
#include "app.h"
#include "flog.h"
#include "pmu.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"


static  app_t                   app;
static  flog_t                  flog;
static  gnss_t                  gnss;
        time_t                  time_dat        =   0;
        USBD_HandleTypeDef      husbd;
        //TIM_HandleTypeDef       htim_cdc;

static  uint8_t                 data_uart1_tx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
static  uint8_t                 data_uart1_rx[ 2 ][ CFG_GNSS_BLCK_SIZE_OCT/2 ];
static  uint8_t                 data_uart2_rx[ 2 ][ CFG_GNSS_UART2_BLCK_SIZE_OCT/2 ];
static  uint8_t                 data_uart3_rx[ 2 ][ CFG_GNSS_UART3_BLCK_SIZE_OCT/2 ];

        fifo_t                  fifo_uart1_tx   =   {   .data     =   data_uart1_tx[0],
                                                        .size     =   0,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };

        fifo_t                  fifo_uart1_rx   =   {   .data     =   data_uart1_rx[0],
                                                        .size     =   CFG_GNSS_BLCK_SIZE_OCT,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };

        fifo_t                  fifo_uart2_rx   =   {   .data     =   data_uart2_rx[0],
                                                        .size     =   CFG_GNSS_UART2_BLCK_SIZE_OCT,
                                                        .head     =   0,
                                                        .tile     =   0,
                                                        .overcome =   false };

        fifo_t                  fifo_uart3_rx   =   {   .data     =   data_uart3_rx[0],
                                                        .size     =   CFG_GNSS_UART3_BLCK_SIZE_OCT,
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
	//osc.PLL.PLLN            =   200;
	osc.PLL.PLLN            =   192;
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
  * @brief  TIM_Config: Configure TIMx timer
  * @param  None.
  * @retval None
  */
/*
static
void TIM_Config( void )
{
	//Set TIMx instance
	htim_cdc.Instance               =   TIM3;

	//Initialize TIM3 peripheral as follow:
	//	+ Period = 10000 - 1
	//	+ Prescaler = ((SystemCoreClock/2)/10000) - 1
	//	+ ClockDivision = 0
	//	+ Counter direction = Up

	//htim_cdc.Init.Period            =   (CDC_POLLING_INTERVAL * 1000) - 1;
	htim_cdc.Init.Period            =   5000;
	//htim_cdc.Init.Prescaler         =   84-1;
	htim_cdc.Init.Prescaler         =   ( (SystemCoreClock / 2) / 10000 ) - 1;
	htim_cdc.Init.ClockDivision     =   0;
	htim_cdc.Init.CounterMode       =   TIM_COUNTERMODE_UP;

	bsp_usb_cdc_init();

	if( HAL_TIM_Base_Init( &htim_cdc ) != HAL_OK )
	{
		app_error();
	}
}
*/

/**
 * @brief
 */
void HAL_UART_RxHalfCpltCallback(               UART_HandleTypeDef *    huart )
{
        if(      huart->Instance == USART1 )
        {
                uint8_t *       data    =   fifo_uart1_rx.data + 0;
                size_t          size    =   CFG_GNSS_BLCK_SIZE_OCT/2;

                if( flog.sts.enable )
                {
                        ui_led_sd_set( false );
                        flog_write( &flog, data, size );
                        ui_led_sd_set( true );
                }

                gnss_read( &gnss, data, size );
        }
        else if( huart->Instance == USART2 )
        {
                //bsp_mcu_uart3_xmit_start( data_uart2_rx[0], CFG_GNSS_UART2_BLCK_SIZE_OCT/2 );
        }
        else if( huart->Instance == USART3 )
        {
                //bsp_mcu_uart2_xmit_start( data_uart3_rx[0], CFG_GNSS_UART3_BLCK_SIZE_OCT/2 );
        }
}

/**
 * @brief
 */
void HAL_UART_RxCpltCallback(                   UART_HandleTypeDef *    huart )
{

        if(      huart->Instance == USART1 )
        {
                uint8_t *       data    =   fifo_uart1_rx.data + CFG_GNSS_BLCK_SIZE_OCT/2;
                size_t          size    =   CFG_GNSS_BLCK_SIZE_OCT/2;

                fifo_uart1_rx.overcome  =   true;

                if( flog.sts.enable )
                {
                        ui_led_sd_set( false );
                        flog_write( &flog, data, size );
                        ui_led_sd_set( true );
                }

                gnss_read( &gnss, data, size );
        }
        else if( huart->Instance == USART2 )
        {
                fifo_uart2_rx.overcome  =   true;
                //bsp_mcu_uart3_xmit_start( data_uart2_rx[1], CFG_GNSS_UART2_BLCK_SIZE_OCT/2 );
        }
        else if( huart->Instance == USART3 )
        {
                fifo_uart3_rx.overcome  =   true;
                //bsp_mcu_uart2_xmit_start( data_uart3_rx[1], CFG_GNSS_UART3_BLCK_SIZE_OCT/2 );
        }

}

static
void app_uart2_rx_hook(                         fifo_t *        p )
{
        size_t                  tile    =   p->tile;
        size_t                  size    =   bsp_mcu_uart2_recv_dma_get();
        size_t                  head    =   p->size - size;


        if( head >= p->size )
        {
                head            =   0;
        }

        if( head > tile )
        {
                size            =   head - tile;
                bsp_mcu_uart3_xmit_start( p->data + tile, size );
                p->tile         +=  size;
        }
        else if( head < tile )
        {
                if( p->overcome )
                {
                        p->overcome     = false;
                        size            =   p->size - tile;
                        bsp_mcu_uart3_xmit_start( p->data + tile, size );
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

static
void app_uart3_rx_hook(                         fifo_t *                p )
{
        //size_t                  tile    =   p->tile;
        size_t                  size    =   bsp_mcu_uart3_recv_dma_get();
        size_t                  head    =   p->size - size;

        if( head >= p->size )
        {
                head            =   0;
        }

        if( head > p->tile )
        {
                size            =   head - p->tile;
                bsp_mcu_uart2_xmit_start( p->data + p->tile, size );
                p->tile         +=  size;
        }
        else if( head < p->tile )
        {
                if( p->overcome )
                {
                        p->overcome     = false;
                        size            =   p->size - p->tile;
                        bsp_mcu_uart2_xmit_start( p->data + p->tile, size );
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
/*
void HAL_TIM_PeriodElapsedCallback(             TIM_HandleTypeDef *     htim )
{
        //gnss_uart_rx_hook( &fifo_uart1_rx );
        //app_uart2_rx_hook( &fifo_uart2_rx );
        //app_uart3_rx_hook( &fifo_uart3_rx );
}
*/

/**
 * @brief
 */
void app_systick_hook( void )
{
	app.evt.ui_key_pwr  =    ui_key_pwr_hook() ? true : false;
	app.evt.ui_key_func =    ui_key_func_hook() ? true : false;

	if( ++(app.tick_1hz) > BSP_SYSTICK_HZ )
	{
		app.tick_1hz        =    0;
		app.evt.tick_1hz    =    true;

		time_dat++;

		gnss_time_sync( &gnss, &time_dat );

		#ifndef	NDEBUG
		app.tick_1hz_cnt++;
		#endif
	}

	if( ++(app.cdc_tick) > (CFG_USB_CDC_TICK_mSEC * BSP_SYSTICK_HZ)/1000 )
	{
		app.cdc_tick        =    0;

                gnss_uart_rx_hook( &fifo_uart1_rx );
                app_uart2_rx_hook( &fifo_uart2_rx );
                app_uart3_rx_hook( &fifo_uart3_rx );
	}
}

/**
 * @brief App entry
 */
int main( void )
{
	#if defined( NDEBUG )
	//skip debugger setup
	#else
	HAL_DBGMCU_EnableDBGSleepMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGStandbyMode();
	#endif

	app_clock_config();
	SysTick_Config(SystemCoreClock / BSP_SYSTICK_HZ);
	HAL_Init();

	ui_init();
	ui_led_sd_set(          false                   );
//ui_led_sd_set(          true                    );
	ui_led_usb_set(         false                   );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_NONE   );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_BLACK  );

	pmu_init();
	pmu_ctl( PMU_CTL_GNSS_LDO_ON, false );

	__enable_irq();

	HAL_Delay( UI_KEY_LONG_TCKS );

	if( ui_key_pwr_forced() )
	{
		//while( ui.key[0].get() == true );
	}
	else
	{
		#if defined( NDEBUG )
		pmu_ctl( PMU_CTL_MCU_OFF, false );
		#endif
	}

	ui_led_sd_flash(        UI_LED_FLSH_LONG_TCKS );
	ui_led_usb_flash(       UI_LED_FLSH_LONG_TCKS );
	ui_led_gnss_flash(      UI_LED_FLSH_LONG_TCKS );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_WHITE );

	HAL_Delay( CFG_UI_LED_FLSH_LONG_mSEC );

	ui_key_pwr_reset();

/*
	TIM_Config();

	if( HAL_TIM_Base_Start_IT( &htim_cdc ) != HAL_OK )
	{
		app_error();
	}
*/

	flog_init( &flog );

	pmu_ctl( PMU_CTL_GNSS_LDO_ON, true );

	USBD_Init(                      &husbd, &VCP_Desc,      0);
	USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
	USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
	USBD_Start( &husbd );

	gnss_init( &gnss );
	gnss_recv_start( fifo_uart1_rx.data, CFG_GNSS_BLCK_SIZE_OCT );


        bsp_mcu_uart2_init( 115200 );
        bsp_mcu_uart3_init( 115200 );

        bsp_mcu_uart2_recv_start( data_uart2_rx[0], CFG_GNSS_UART2_BLCK_SIZE_OCT );
        bsp_mcu_uart3_recv_start( data_uart3_rx[0], CFG_GNSS_UART3_BLCK_SIZE_OCT );


	while( true )
	{
		if( app.evt.ui_key_pwr )
		{
			app.evt.ui_key_pwr      =    false;

			switch( ui_key_pwr_status() )
			{
				case UI_KEY_STS_SHORT:
					//ui_led_pwr_flash( UI_LED_FLSH_SHRT_TCKS );
					//gnss_send( &gnss, CFG_GNSS_MSG_KEY0S );
					break;

				case UI_KEY_STS_LONG:
					pmu_ctl( PMU_CTL_MCU_OFF, false );
					break;

				default:
					break;
			}
		}

		if( app.evt.ui_key_func )
		{
			app.evt.ui_key_func     =    false;

			switch( ui_key_func_status() )
			{
				case UI_KEY_STS_SHORT:
					gnss_send( &gnss, CFG_GNSS_MSG_KEY1S );
					break;

				case UI_KEY_STS_LONG:
					if( flog.sts.enable )
					{
						flog_close( &flog );
					}
					else
					{
						flog_open( &flog );
					}

					ui_led_sd_set( flog.sts.enable );

					break;

				default:
					break;
			}
		}

		if( app.evt.tick_1hz )
		{
			app.evt.tick_1hz    =   false;

			switch( gnss.nmea.gga.fix )
			{
				case NMEA_FIX_RTK_FLT:	ui_led_gnss_set( UI_LED_GNSS_MODE_RTKFLT ); break;
				case NMEA_FIX_RTK_INT:  ui_led_gnss_set( UI_LED_GNSS_MODE_RTKINT ); break;
				case NMEA_FIX_PPS:      ui_led_gnss_set( UI_LED_GNSS_MODE_DGPS   ); break;
				case NMEA_FIX_DGPS:     ui_led_gnss_set( UI_LED_GNSS_MODE_DGPS   ); break;
				case NMEA_FIX_SPS:      ui_led_gnss_set( UI_LED_GNSS_MODE_GPS    ); break;
				case NMEA_FIX_NONE:     ui_led_gnss_set( UI_LED_GNSS_MODE_NONE   ); break;
				default:                ui_led_gnss_set( UI_LED_GNSS_MODE_NONE   ); break;
			}

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
