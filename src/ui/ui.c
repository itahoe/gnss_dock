/**
  * @file    ui.h
  * @brief   User Interface
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_key.h"
#include	"bsp_led.h"
#include	"ui.h"

	ui_key_t        ui_key_pwr      =   {   .init           =   bsp_key0_init,
	                                        .get            =   bsp_key0_get,
	                                        .status         =   UI_KEY_STS_NONE,
	                                        .tick           =   0 };

	ui_key_t        ui_key_func     =   {   .init           =   bsp_key1_init,
	                                        .get            =   bsp_key1_get,
	                                        .status         =   UI_KEY_STS_NONE,
	                                        .tick           =   0 };


	ui_led_t        ui_led_sd       =   {   .init           =   bsp_led_sd_init,
	                                        .get            =   bsp_led_sd_get,
	                                        .set            =   bsp_led_sd_set,
	                                        .tick           =   0 };

	ui_led_t        ui_led_usb      =   {   .init           =   bsp_led_usb_init,
	                                        .get            =   bsp_led_usb_get,
	                                        .set            =   bsp_led_usb_set,
	                                        .tick           =   0 };

	ui_led_gnss_t   ui_led_gnss     =   {   .init           =   bsp_led_gnss_init,
	                                        .get            =   bsp_led_gnss_get,
	                                        .set            =   bsp_led_gnss_set,
	                                        .tick           =   0,
                                                .mode           =   UI_LED_GNSS_MODE_NONE };

	ui_led_t        ui_led_pwr      =   {   .init           =   bsp_led_pwr_init,
	                                        .get            =   bsp_led_pwr_get,
	                                        .set            =   bsp_led_pwr_set,
	                                        .tick           =   0 };

/**
 * @brief UI initialization.
 */
void	ui_init( void )
{
	ui_led_sd.init();
	ui_led_usb.init();
        ui_led_gnss.init();
	ui_led_pwr.init();

	ui_key_pwr.init();
	ui_key_func.init();
}
