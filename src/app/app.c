/**
  * @file    app.c
  * @brief   App
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include "ui.h"
#include "gnss.h"
#include "app.h"
#include "flog.h"
#include "pmu.h"
#include "usbd_desc.h"
#include "usbd_cdc.h" 
#include "usbd_cdc_interface.h"


	app_t                   app;
	flog_t                  flog;
extern	gnss_fifo_t             gnss_data_uart_rx;
	gnss_t                  gnss;
	USBD_HandleTypeDef      husbd;


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
void	app_clock_config( void )
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
	//osc.PLL.PLLM            =   16;
	osc.PLL.PLLM            =   32;
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
/*
	//Enable HSE Oscillator and activate PLL with HSE as source
	osc.OscillatorType      =   RCC_OSCILLATORTYPE_HSE;
	osc.HSEState            =   RCC_HSE_ON;
	osc.PLL.PLLState        =   RCC_PLL_ON;
	osc.PLL.PLLSource       =   RCC_PLLSOURCE_HSE;
	//osc.PLL.PLLM            =   8;
	osc.PLL.PLLM            =   16;
	osc.PLL.PLLN            =   360;
	osc.PLL.PLLP            =   RCC_PLLP_DIV2;
	osc.PLL.PLLQ            =   7;
	osc.PLL.PLLR            =   2;
	HAL_RCC_OscConfig( &osc );
*/
	/* Activate the OverDrive to reach the 180 MHz Frequency */  
	HAL_PWREx_EnableOverDrive();

	/* Select PLLSAI output as USB clock source */
	//PeriphClkInitStruct.PLLSAI.PLLSAIM          =   8;
	PeriphClkInitStruct.PLLSAI.PLLSAIM          =   16;
	PeriphClkInitStruct.PLLSAI.PLLSAIN          =   384;
	PeriphClkInitStruct.PLLSAI.PLLSAIP          =   RCC_PLLSAIP_DIV8;
	PeriphClkInitStruct.PeriphClockSelection    =   RCC_PERIPHCLK_CK48;
	PeriphClkInitStruct.Clk48ClockSelection     =   RCC_CK48CLKSOURCE_PLLSAIP;
	HAL_RCCEx_PeriphCLKConfig( &PeriphClkInitStruct );
/*
	//Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers
	clk.ClockType            =   (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	clk.SYSCLKSource         =   RCC_SYSCLKSOURCE_PLLCLK;
	clk.AHBCLKDivider        =   RCC_SYSCLK_DIV1;
	clk.APB1CLKDivider       =   RCC_HCLK_DIV4;
	clk.APB2CLKDivider       =   RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_5 );
*/

	SystemCoreClockUpdate();
}

/**
 * @brief
 */
void	HAL_UART_RxHalfCpltCallback(    UART_HandleTypeDef *    huart )
{
	uint8_t *       data    =   gnss_data_uart_rx.data + 0;
	size_t          size    =   CFG_GNSS_BLCK_SIZE_OCT/2;

/*
	if( flog.sts.enable )
	{
                ui_led_sd_set( false );
		flog_write( &flog, data, size );
                ui_led_sd_set( true );
	}
*/
        app.evt.data0_ready     =   true;

	gnss_read( &gnss, data, size );
}

/**
 * @brief
 */
void	HAL_UART_RxCpltCallback(        UART_HandleTypeDef *	huart )
{
	uint8_t *       data    =   gnss_data_uart_rx.data + CFG_GNSS_BLCK_SIZE_OCT/2;
	size_t          size    =   CFG_GNSS_BLCK_SIZE_OCT/2;


        gnss_data_uart_rx.overcome      =   true;
/*
	if( flog.sts.enable )
	{
                ui_led_sd_set( false );
		flog_write( &flog, data, size );
                ui_led_sd_set( true );
	}
*/
        app.evt.data1_ready     =   true;

	gnss_read( &gnss, data, size );
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

	flog_init( &flog );

	pmu_ctl( PMU_CTL_GNSS_LDO_ON, true );

	USBD_Init(                      &husbd, &VCP_Desc,      0);
	USBD_RegisterClass(             &husbd, USBD_CDC_CLASS );
	USBD_CDC_RegisterInterface(     &husbd, &USBD_CDC_fops );
	USBD_Start( &husbd );

	gnss_init( &gnss );
	gnss_ctl( GNSS_CTL_RECV_START );

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

		if( app.evt.data0_ready )
		{
			app.evt.data0_ready     =   false;

                        if( flog.sts.enable )
                        {
                                ui_led_sd_set( false );
                                flog_write( &flog, gnss_data_uart_rx.data + 0, CFG_GNSS_BLCK_SIZE_OCT/2 );
                                ui_led_sd_set( true );
                        }
		}

		if( app.evt.data1_ready )
		{
			app.evt.data1_ready     =   false;

                        if( flog.sts.enable )
                        {
                                ui_led_sd_set( false );
                                flog_write( &flog, gnss_data_uart_rx.data + CFG_GNSS_BLCK_SIZE_OCT/2, CFG_GNSS_BLCK_SIZE_OCT/2 );
                                ui_led_sd_set( true );
                        }
		}
	}
}
