/**
  * @file    ui.h
  * @brief   User Interface
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_key.h"
#include	"bsp_led.h"
#include	"ui.h"

	ui_key_t        ui_key_pwr              =   {   .init   =   bsp_key0_init,
	                                                .get    =   bsp_key0_get,
	                                                .status =   UI_KEY_STS_NONE,
	                                                .tick   =   0 };

	ui_key_t        ui_key_func             =   {   .init   =   bsp_key1_init,
	                                                .get    =   bsp_key1_get,
	                                                .status =   UI_KEY_STS_NONE,
	                                                .tick   =   0 };


	ui_led_t        ui_led_sd               =   {   .init   =   bsp_led_sd_init,
	                                                .get    =   bsp_led_sd_get,
	                                                .set    =   bsp_led_sd_set,
	                                                 .tick   =   0 };

	ui_led_t        ui_led_usb              =   {   .init   =   bsp_led_usb_init,
	                                                .get    =   bsp_led_usb_get,
	                                                .set    =   bsp_led_usb_set,
	                                                .tick   =   0 };

	ui_led_sp4t_t   ui_led_gnss	        =   {   .gps.init       =   bsp_led_gnss_gps_init,
	                                                .gps.get        =   bsp_led_gnss_gps_get,
	                                                .gps.set        =   bsp_led_gnss_gps_set,
	                                                .gps.tick       =   UI_LED_FLSH_SHRT_TCKS * 1,

	                                                .dgps.init      =   bsp_led_gnss_dgps_init,
	                                                .dgps.get       =   bsp_led_gnss_dgps_get,
	                                                .dgps.set       =   bsp_led_gnss_dgps_set,
	                                                .dgps.tick      =   UI_LED_FLSH_SHRT_TCKS * 2,

	                                                .rtkint.init    =   bsp_led_gnss_rtkint_init,
	                                                .rtkint.get     =   bsp_led_gnss_rtkint_get,
	                                                .rtkint.set     =   bsp_led_gnss_rtkint_set,
	                                                .rtkint.tick    =   UI_LED_FLSH_SHRT_TCKS * 3,

	                                                .rtkflt.init    =   bsp_led_gnss_rtkflt_init,
	                                                .rtkflt.get     =   bsp_led_gnss_rtkflt_get,
	                                                .rtkflt.set     =   bsp_led_gnss_rtkflt_set,
	                                                .rtkflt.tick    =   UI_LED_FLSH_SHRT_TCKS * 4 };

	ui_led_rgb_t    ui_led_pwr              =   {   .r.init         =   bsp_led_pwr_r_init,
	                                                .r.get          =   bsp_led_pwr_r_get,
	                                                .r.set          =   bsp_led_pwr_r_set,
	                                                .r.tick         =   0,

	                                                .g.init         =   bsp_led_pwr_g_init,
	                                                .g.get          =   bsp_led_pwr_g_get,
	                                                .g.set          =   bsp_led_pwr_g_set,
	                                                .g.tick         =   0,

	                                                .b.init         =   bsp_led_pwr_b_init,
	                                                .b.get          =   bsp_led_pwr_b_get,
	                                                .b.set          =   bsp_led_pwr_b_set,
	                                                .b.tick         =   0 };

/**
 * @brief UI initialization.
 */
void	ui_init( void )
{
	ui_led_sd.init();

	ui_led_usb.init();

	ui_led_gnss.gps.init();
	ui_led_gnss.dgps.init();
	ui_led_gnss.rtkint.init();
	ui_led_gnss.rtkflt.init();

	ui_led_pwr.r.init();
	ui_led_pwr.g.init();
	ui_led_pwr.b.init();

	ui_key_pwr.init();
	ui_key_func.init();
}
