/**
  * @file    ui_led.c
  * @brief   User Interface - LED control service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"

/**
 * @brief UI LED flash.
 */
static
void ui_led_flash(                      ui_led_t *              p,
                                        size_t                  len )
{
	if( p->tick == 0 )
	{
		ui_led_toggle( p );
	}

	p->tick         =   len;
}

/**
 * @brief UI LED toggle.
 */
void ui_led_toggle(                     ui_led_t *              p )
{
	const   bool    led_sts =   p->get();

	p->set( led_sts ? false : true );
}

/**
 * @brief UI LED Control.
 */
void ui_led_set(                        ui_led_t *              p,
                                const   ui_led_mode_t           mode )
{
	switch( mode )
	{
		case UI_LED_MODE_OFF:       p->set( false );                            break;
		case UI_LED_MODE_ON:        p->set( true );                             break;
		case UI_LED_MODE_TOGGLE:    ui_led_toggle( p );                         break;
		case UI_LED_MODE_FLSH_SHRT: ui_led_flash( p, UI_LED_FLSH_SHRT_TCKS );   break;
		case UI_LED_MODE_FLSH_LONG: ui_led_flash( p, UI_LED_FLSH_LONG_TCKS );   break;
		default:
			break;
	}
}

/**
 * @brief UI LED hook.
 */
bool ui_led_hook(                       ui_led_t *              p )
{
	bool            resp            =   false;

	if( p->tick > 0 )
	{
		p->tick--;

		if( p->tick == 0 )
		{
			ui_led_set( p, UI_LED_MODE_TOGGLE );
			resp    =   true;
		}
	}

	return( resp );
}
