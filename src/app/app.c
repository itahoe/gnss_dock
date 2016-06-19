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


	gnss_t                  gnss;
	app_t                   app;
	flog_t                  flog;
	USBD_HandleTypeDef      USBD_Device;
extern	ui_t                    ui;
extern	uint8_t                 uart_data_xmit[];
extern	uint8_t                 uart_data_recv[];


/**
  * @brief  This function is executed in case of error occurrence.
  */
void	app_error( void )
{
	uint32_t        delay_msec  =   1000;

	while( true )
	{
		ui_led_sd_set( UI_LED_MODE_TOGGLE );
		ui_led_usb_set( UI_LED_MODE_TOGGLE );
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

#define APP_TX_DATA_SIZE  2048

extern	uint8_t		*UserTxBuffer;
extern	uint32_t        UserTxBufPtrIn;

static
void app_usb_cdc_send(                  uint8_t *               data,
                                        size_t                  size )
{
	//HAL_UART_Receive_IT( huart, (uint8_t *)(UserTxBuffer + UserTxBufPtrIn), 1);
/*
	while( size-- )
	{
		*(UserTxBuffer + UserTxBufPtrIn++)	=  *data++;

		if( UserTxBufPtrIn >= APP_TX_DATA_SIZE )
		{
			UserTxBufPtrIn          =   0;
		}
	}
*/

	USBD_CDC_SetTxBuffer(   &USBD_Device,
	                        data,
	                        size );
    
	USBD_CDC_TransmitPacket( &USBD_Device );
}

/**
 * @brief
 */
void	HAL_UART_RxHalfCpltCallback(    UART_HandleTypeDef *    huart )
{
	if( flog.sts.enable )
	{
		ui_led_sd_set( UI_LED_MODE_FLSH_SHRT );

		flog_write(     &flog,
		                uart_data_recv + 0,
		                CFG_FMNG_BLCK_SIZE_OCT/2 );
	}

	gnss_recv(	&gnss,
	                uart_data_recv + 0,
	                CFG_FMNG_BLCK_SIZE_OCT/2 );

	app_usb_cdc_send( uart_data_recv + 0,
	                  CFG_FMNG_BLCK_SIZE_OCT/2 );
}

/**
 * @brief
 */
void	HAL_UART_RxCpltCallback(        UART_HandleTypeDef *	huart )
{
	if( flog.sts.enable )
	{
		ui_led_sd_set( UI_LED_MODE_FLSH_SHRT );

		flog_write(     &flog,
		                uart_data_recv + CFG_FMNG_BLCK_SIZE_OCT/2,
		                CFG_FMNG_BLCK_SIZE_OCT/2 );
	}

	gnss_recv(	&gnss,
	                uart_data_recv + CFG_FMNG_BLCK_SIZE_OCT/2,
	                CFG_FMNG_BLCK_SIZE_OCT/2 );

	app_usb_cdc_send( uart_data_recv + CFG_FMNG_BLCK_SIZE_OCT/2,
	                  CFG_FMNG_BLCK_SIZE_OCT/2 );
}

/**
 * @brief App entry
 */
int main( void )
{
	#if defined( NDEBUG )
	//do nothing
	#else
	HAL_DBGMCU_EnableDBGSleepMode();
	HAL_DBGMCU_EnableDBGStopMode();
	HAL_DBGMCU_EnableDBGStandbyMode();
	#endif

	app_clock_config();
	SysTick_Config(SystemCoreClock / BSP_SYSTICK_HZ);
	HAL_Init();
	__enable_irq();

	ui_init();

	ui_led_sd_set(          UI_LED_MODE_OFF         );
	ui_led_usb_set(         UI_LED_MODE_OFF         );
	ui_led_gnss_set(        UI_LED_GNSS_MODE_NONE   );
	ui_led_pwr_set(         UI_LED_RGB_COLOR_BLACK  );

	pmu_init();
	pmu_ctl( PMU_CTL_LDO, false );

	HAL_Delay( UI_KEY_LONG_TCKS );

	if( ui.key[0].get() == true )
	{
		//while( ui.key[0].get() == true );
	}
	else
	{
		#if defined( NDEBUG )
		pmu_ctl( PMU_CTL_MCU_OFF, false );
		#endif
	}

	ui_led_sd_set( UI_LED_MODE_FLSH_LONG );
	ui_led_usb_set( UI_LED_MODE_FLSH_LONG );

	ui_led_pwr_set( UI_LED_RGB_COLOR_WHITE );


ui_led_gnss_set( UI_LED_GNSS_MODE_GPS );
HAL_Delay( 500 );
ui_led_gnss_set( UI_LED_GNSS_MODE_DGPS );
HAL_Delay( 500 );
ui_led_gnss_set( UI_LED_GNSS_MODE_RTKINT );
HAL_Delay( 500 );
ui_led_gnss_set( UI_LED_GNSS_MODE_RTKFLT );
HAL_Delay( 500 );
	ui_led_gnss_set( UI_LED_GNSS_MODE_NONE );


ui_led_pwr_set( UI_LED_RGB_COLOR_RED );
HAL_Delay( 1000 );

	ui.key[0].status        =   UI_KEY_STS_NONE;
	ui.key[0].tick          =   0;


	flog_init( &flog );


	if( flog.sts.ready )
	{
		ui_led_sd_set( UI_LED_MODE_ON );
	}
	else
	{
		ui_led_sd_set( UI_LED_MODE_OFF );
	}

ui_led_pwr_set( UI_LED_RGB_COLOR_GREEN );
HAL_Delay( 1000 );

	gnss_init( &gnss );

ui_led_pwr_set( UI_LED_RGB_COLOR_BLUE );
HAL_Delay( 1000 );

	pmu_ctl( PMU_CTL_LDO, true );

HAL_Delay( 1000 );

	gnss_ctl( GNSS_CTL_RECV_START );

	ui_led_pwr_set( UI_LED_RGB_COLOR_WHITE );
	HAL_Delay( 1000 );


	USBD_Init(                      &USBD_Device,   &VCP_Desc,      0); //Init Device Library
	USBD_RegisterClass(             &USBD_Device,   USBD_CDC_CLASS ); //Add Supported Class
	USBD_CDC_RegisterInterface(     &USBD_Device,   &USBD_CDC_fops ); //Add CDC Interface Class
	USBD_Start( &USBD_Device ); //Start Device Process 

HAL_Delay( 1000 );

	while( true )
	{
		if( app.evt.ui_key0 )
		{
			app.evt.ui_key0 =    false;

			switch( ui.key[0].status )
			{
				case UI_KEY_STS_SHORT:
					ui_led_pwr_flash( UI_LED_RGB_FLSH_SHRT );

					//gnss_send( &gnss, CFG_GNSS_MSG_KEY0S );
					break;

				case UI_KEY_STS_LONG:
					pmu_ctl( PMU_CTL_MCU_OFF, false );
					break;

				default:
					break;
			}
		}

		if( app.evt.ui_key1 )
		{
			app.evt.ui_key1 =    false;

			switch( ui.key[1].status )
			{
				case UI_KEY_STS_SHORT:
					ui_led_pwr_flash( UI_LED_RGB_FLSH_SHRT );
					//gnss_send( &gnss, CFG_GNSS_MSG_KEY1S );
					break;

				case UI_KEY_STS_LONG:
					if( flog.sts.enable )
					{
						flog.sts.enable         =  false;
						flog_close( &flog );
						ui_led_sd_set( UI_LED_MODE_ON );
					}
					else
					{
						flog_open( &flog );
						flog.sts.enable         =  true;
						ui_led_sd_set( UI_LED_MODE_OFF );
					}
					break;

				default:
					break;
			}
		}

		if( app.evt.tick_1hz )
		{
			app.evt.tick_1hz    =   false;

			ui_led_sd_set( flog.sts.enable ? UI_LED_MODE_ON : UI_LED_MODE_OFF );

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

			ui_led_pwr_flash( UI_LED_RGB_FLSH_SHRT );
/*
			APP_TRACE(	"ERR: %d, PE: %d, FE: %d, NE: %d, ORE: %d, IDLE: %d\n",
			                gnss.nmea.chksum_errors,
			                app.uart1.parity_error,
			                app.uart1.framing_error,
			                app.uart1.noise_error,
			                app.uart1.overrun_error,
			                app.uart1.idle_line );
*/
		}

		if( app.evt.nmea )
		{
			app.evt.nmea        =   false;
		}

		if( app.evt.log_write )
		{
			app.evt.log_write   =   false;

			//fmng_write( &fmng, fmng.buf_full, CFG_FMNG_BLCK_SIZE_OCT );
		}
	}
}
