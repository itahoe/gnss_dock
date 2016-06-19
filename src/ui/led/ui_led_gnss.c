/**
  * @file    ui_led_gnss.c
  * @brief   User Interface, LED services, GNSS, Single Poll - Four Throw
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"


extern	ui_led_sp4t_t           ui_led_gnss;



/**
 * @brief GNSS LED's batch control
 */
void ui_led_gnss_set(           const   ui_led_gnss_mode_t      mode )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;
	bool            gps     =   false;
	bool            dpgs    =   false;
	bool            rtkint  =   false;
	bool            rtkflt  =   false;


	p->mode         =   mode;

	switch( p->mode )
	{
		case UI_LED_GNSS_MODE_RTKFLT:   rtkflt = true;  break;
		case UI_LED_GNSS_MODE_RTKINT:   rtkint = true;  break;
		case UI_LED_GNSS_MODE_DGPS:     dpgs   = true;  break;
		case UI_LED_GNSS_MODE_GPS:      gps    = true;  break;
		case UI_LED_GNSS_MODE_NONE:
		default:
			break;
	}

	p->gps.set(     gps    );
	p->dgps.set(    dpgs   );
	p->rtkint.set(  rtkint );
	p->rtkflt.set(  rtkflt );
}

void ui_led_gnss_hook( void )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;

	ui_led_hook( &p->gps );
	ui_led_hook( &p->dgps );
	ui_led_hook( &p->rtkint );
	ui_led_hook( &p->rtkflt );
}