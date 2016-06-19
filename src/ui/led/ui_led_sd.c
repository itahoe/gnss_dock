/**
  * @file    ui_led_sd.c
  * @brief   User Interface - SD LED control service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"


extern	ui_led_t                ui_led_sd;


/**
 * @brief SD LED Control
 */
void ui_led_sd_set(             const   ui_led_mode_t           mode )
{
	ui_led_set( &ui_led_sd, mode );
}

/**
 * @brief SD LED hook.
 */
bool ui_led_sd_hook( void )
{
	bool            resp    =   ui_led_hook( &ui_led_sd );

	return( resp );
}
