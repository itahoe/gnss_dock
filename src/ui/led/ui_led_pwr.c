/**
  * @file    ui_led_pwr.c
  * @brief   User Interface - PWR LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"


extern	ui_led_rgb_t            ui_led_pwr;


void ui_led_pwr_set(            const   ui_led_rgb_color_t      color )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;
	bool            r, g, b;


	p->color        =   color;

	switch( p->color )
	{
		case UI_LED_RGB_COLOR_WHITE:    r = true;  g = true;  b = true;  break;
		case UI_LED_RGB_COLOR_MAGENTA:  r = true;  g = false; b = true;  break;
		case UI_LED_RGB_COLOR_CYAN:     r = false; g = true;  b = true;  break;
		case UI_LED_RGB_COLOR_YELLOW:   r = true;  g = true;  b = false; break;
		case UI_LED_RGB_COLOR_BLUE:     r = false; g = false; b = true;  break;
		case UI_LED_RGB_COLOR_GREEN:    r = false; g = true;  b = false; break;
		case UI_LED_RGB_COLOR_RED:      r = true;  g = false; b = false; break;
		case UI_LED_RGB_COLOR_BLACK:    r = false; g = false; b = false; break;
		default:                        r = false; g = false; b = false; break;
	}

	p->r.set( r );
	p->g.set( g );
	p->b.set( b );
}

void ui_led_pwr_flash(          const   ui_led_rgb_flsh_t       flash )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;
	bool            r, g, b;
        ui_led_mode_t   mode;


	switch( p->color )
	{
		case UI_LED_RGB_COLOR_WHITE:    r = true;  g = true;  b = true;  break;
		case UI_LED_RGB_COLOR_MAGENTA:  r = true;  g = false; b = true;  break;
		case UI_LED_RGB_COLOR_CYAN:     r = false; g = true;  b = true;  break;
		case UI_LED_RGB_COLOR_YELLOW:   r = true;  g = true;  b = false; break;
		case UI_LED_RGB_COLOR_BLUE:     r = false; g = false; b = true;  break;
		case UI_LED_RGB_COLOR_GREEN:    r = false; g = true;  b = false; break;
		case UI_LED_RGB_COLOR_RED:      r = true;  g = false; b = false; break;
		case UI_LED_RGB_COLOR_BLACK:    r = true;  g = true;  b = true;  break;
		default:                        r = false; g = false; b = false; break;
	}

	switch( flash )
	{
		case UI_LED_RGB_FLSH_LONG:      mode = UI_LED_MODE_FLSH_LONG;    break;
		case UI_LED_RGB_FLSH_SHRT:      mode = UI_LED_MODE_FLSH_SHRT;    break;
		default:                        mode = UI_LED_MODE_SKIP;         break;
	}

	if( r )
	{
		ui_led_set( &p->r, mode );
	}

	if( g )
	{
		ui_led_set( &p->g, mode );
	}

	if( b )
	{
		ui_led_set( &p->b, mode );
	}
}

void ui_led_pwr_hook( void )
{
	ui_led_rgb_t *  p       =  &ui_led_pwr;

	ui_led_hook( &p->r );
	ui_led_hook( &p->g );
	ui_led_hook( &p->b );
}
