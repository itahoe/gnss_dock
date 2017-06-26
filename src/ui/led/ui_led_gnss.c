/**
  * @file    ui_led_gnss.c
  * @brief   UI - GNSS LED's batch (Single Poll - Four Throw)
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"ui_led.h"

//extern	ui_led_rgb_t            ui_led_gnss;
extern  ui_led_t                ui_led_gnss;

/**
 * @brief GNSS LED SysTick Hook.
 */
/*
void ui_led_gnss_hook( void )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;

	ui_led_hook( &p->gps );
	ui_led_hook( &p->dgps );
	ui_led_hook( &p->rtkint );
	ui_led_hook( &p->rtkflt );
}
*/

/**
 * @brief GNSS LED Toggle.
 */
/*
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
*/

/**
 * @brief GNSS LED Control.
 */
/*
void ui_led_gnss_set(           const   ui_led_gnss_mode_t      mode )
{
	ui_led_sp4t_t * p       =  &ui_led_gnss;
        bool            rtkflt  =   false;
        bool            rtkint  =   false;
        bool            dgps    =   false;
        bool            gps     =   false;


	p->mode         =   mode;

	switch( p->mode )
	{
		case UI_LED_GNSS_MODE_RTKFLT:
			rtkflt          =   true;
			break;

		case UI_LED_GNSS_MODE_RTKINT:
			rtkint          =   true;
			break;

		case UI_LED_GNSS_MODE_DGPS:
			dgps            =   true;
			break;

		case UI_LED_GNSS_MODE_GPS:
			gps             =   true;
			break;

		case UI_LED_GNSS_MODE_NONE:
		default:
			break;
	}

        p->rtkflt.set(  rtkflt  );
        p->rtkint.set(  rtkint  );
        p->dgps.set(    dgps    );
        p->gps.set(     gps     );

}
*/


static
void ui_led_gnss_color_decomp(          bool *                  r,
                                        bool *                  g,
                                        bool *                  b,
                                const   ui_led_rgb_color_t      color )
{
	switch( color )
	{
		case UI_LED_RGB_COLOR_WHITE:    *r = true;  *g = true;  *b = true;  break;
		case UI_LED_RGB_COLOR_MAGENTA:  *r = true;  *g = false; *b = true;  break;
		case UI_LED_RGB_COLOR_CYAN:     *r = false; *g = true;  *b = true;  break;
		case UI_LED_RGB_COLOR_YELLOW:   *r = true;  *g = true;  *b = false; break;
		case UI_LED_RGB_COLOR_BLUE:     *r = false; *g = false; *b = true;  break;
		case UI_LED_RGB_COLOR_GREEN:    *r = false; *g = true;  *b = false; break;
		case UI_LED_RGB_COLOR_RED:      *r = true;  *g = false; *b = false; break;
		case UI_LED_RGB_COLOR_BLACK:    *r = false; *g = false; *b = false; break;
		default:                        *r = false; *g = false; *b = false; break;
	}
}


/**
 * @brief GNSS LED Control.
 */
void ui_led_gnss_set(                   const   int                     mode )
{
/*
#ifdef GNSS_DOCK_HW1629
	ui_led_rgb_t *  p       =  &ui_led_gnss;
	bool            r, g, b;


        switch( mode )
        {
                case UI_LED_GNSS_MODE_RTKINT:
                        p->color        =   UI_LED_RGB_COLOR_MAGENTA;
                        break;

                case UI_LED_GNSS_MODE_RTKFLT:
                        p->color        =   UI_LED_RGB_COLOR_BLUE;
                        break;

                case UI_LED_GNSS_MODE_DGPS:
                        p->color        =   UI_LED_RGB_COLOR_GREEN;
                        break;

                case UI_LED_GNSS_MODE_GPS:
                        p->color        =   UI_LED_RGB_COLOR_RED;
                        break;

                case UI_LED_GNSS_MODE_NONE:
                default:
                        p->color        =   UI_LED_RGB_COLOR_BLACK;
                        break;
        }

	ui_led_gnss_color_decomp( &r, &g, &b, p->color );

	p->r.set( r );
	p->g.set( g );
	p->b.set( b );
#elif GNSS_DOCK_HW1721
#else
#warning "Select appropriaye UI LED GNSS"
#endif
*/



	//ui_led_sp4t_t * p       =  &ui_led_gnss;
        //bool            rtkflt  =   false;
        //bool            rtkint  =   false;
        //bool            dgps    =   false;
        //bool            gps     =   false;

	//p->mode         =   mode;
/*
	switch( p->mode )
	{
		case UI_LED_GNSS_MODE_RTKFLT:
			rtkflt          =   true;
			break;

		case UI_LED_GNSS_MODE_RTKINT:
			rtkint          =   true;
			break;

		case UI_LED_GNSS_MODE_DGPS:
			dgps            =   true;
			break;

		case UI_LED_GNSS_MODE_GPS:
			gps             =   true;
			break;

		case UI_LED_GNSS_MODE_NONE:
		default:
			break;
	}

        p->rtkflt.set(  rtkflt  );
        p->rtkint.set(  rtkint  );
        p->dgps.set(    dgps    );
        p->gps.set(     gps     );
*/

        bsp_led_color_t         color;

        switch( mode )
        {
                case UI_LED_GNSS_MODE_RTKINT:   color   =   BSP_LED_COLOR_GREEN;        break;
                case UI_LED_GNSS_MODE_RTKFLT:   color   =   BSP_LED_COLOR_BLUE;         break;
                case UI_LED_GNSS_MODE_DGPS:     color   =   BSP_LED_COLOR_YELLOW;       break;
                case UI_LED_GNSS_MODE_GPS:      color   =   BSP_LED_COLOR_RED;          break;
                case UI_LED_GNSS_MODE_NONE:
                default:                        color   =   BSP_LED_COLOR_BLACK;        break;
        }

        ui_led_gnss.set( color );
}




/**
 * @brief GNSS LED Flash.
 */
/*
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
*/
