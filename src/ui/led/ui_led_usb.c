/**
  * @file    ui_led_usb.c
  * @brief   User Interface - LED control service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"


extern	ui_led_t                ui_led_usb;


/**
 * @brief USB LED Control
 */
void ui_led_usb_set(            const   ui_led_mode_t           mode )
{
	ui_led_set( &ui_led_usb, mode );
}

/**
 * @brief USB LED hook.
 */
bool	ui_led_usb_hook( void )
{
	bool            resp    =   ui_led_hook( &ui_led_usb );

	return( resp );
}
