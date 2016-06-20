/**
  * @file    ui_led_gnss.c
  * @brief   UI - GNSS LED's batch (Single Poll - Four Throw)
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"

extern	ui_led_sp4t_t           ui_led_gnss;

/**
 * @brief GNSS LED SysTick Hook.
 */
void ui_led_gnss_hook( void )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;

	ui_led_hook( &p->gps );
	ui_led_hook( &p->dgps );
	ui_led_hook( &p->rtkint );
	ui_led_hook( &p->rtkflt );
}

/**
 * @brief GNSS LED Toggle.
 */
void ui_led_gnss_toggle( void )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;


	switch( p->mode )
	{
		case UI_LED_GNSS_MODE_RTKFLT:
			ui_led_toggle( &p->rtkflt );
			break;

		case UI_LED_GNSS_MODE_RTKINT:
			ui_led_toggle( &p->rtkint );
			break;

		case UI_LED_GNSS_MODE_DGPS:
			ui_led_toggle( &p->dgps   );
			break;

		case UI_LED_GNSS_MODE_GPS:
			ui_led_toggle( &p->gps    );
			break;

		case UI_LED_GNSS_MODE_NONE:
			ui_led_toggle( &p->rtkflt );
			ui_led_toggle( &p->rtkint );
			ui_led_toggle( &p->dgps   );
			ui_led_toggle( &p->gps    );
			break;

		default:
			break;
	}
}

/**
 * @brief GNSS LED Control.
 */
void ui_led_gnss_set(           const   ui_led_gnss_mode_t      mode )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;


	p->mode         =   mode;

	switch( p->mode )
	{
		case UI_LED_GNSS_MODE_RTKFLT:
			p->rtkflt.set(  true  );
			break;

		case UI_LED_GNSS_MODE_RTKINT:
			p->rtkint.set(  true  );
			break;

		case UI_LED_GNSS_MODE_DGPS:
			p->dgps.set(    true  );
			break;

		case UI_LED_GNSS_MODE_GPS:
			p->gps.set(     true  );
			break;

		case UI_LED_GNSS_MODE_NONE:
			p->rtkflt.set(  false );
			p->rtkint.set(  false );
			p->dgps.set(    false );
			p->gps.set(     false );
			break;

		default:
			break;
	}
}

/**
 * @brief GNSS LED Flash.
 */
void ui_led_gnss_flash(         const   size_t                  len )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;


	switch( p->mode )
	{
		case UI_LED_GNSS_MODE_RTKFLT:
			ui_led_flash( &p->rtkflt, len );
			break;

		case UI_LED_GNSS_MODE_RTKINT:
			ui_led_flash( &p->rtkint, len );
			break;

		case UI_LED_GNSS_MODE_DGPS:
			ui_led_flash( &p->dgps,   len );
			break;

		case UI_LED_GNSS_MODE_GPS:
			ui_led_flash( &p->gps,    len );
			break;

		case UI_LED_GNSS_MODE_NONE:
			ui_led_flash( &p->rtkflt, len );
			ui_led_flash( &p->rtkint, len );
			ui_led_flash( &p->dgps,   len );
			ui_led_flash( &p->gps,    len );
			break;

		default:
			break;
	}
}