/**
  * @file    ui_led.c
  * @brief   UI - LED.
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"



/**
 * @brief UI LED SysTick Hook.
 */
bool ui_led_hook(                       ui_led_t *              p )
{
	bool            resp            =   false;

	if( p->tick > 0 )
	{
		p->tick--;

		if( p->tick == 0 )
		{
			ui_led_toggle( p );
			resp    =   true;
		}
	}

	return( resp );
}

/**
 * @brief UI LED Toggle.
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
                                const   bool                    mode )
{
	p->set( mode );
}

/**
 * @brief UI LED Flash.
 */
void ui_led_flash(                      ui_led_t *              p,
                                        size_t                  len )
{
	if( p->tick == 0 )
	{
		ui_led_toggle( p );
	}

	p->tick         =   len;
}

