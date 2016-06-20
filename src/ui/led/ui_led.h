/**
  * @file    ui_led.h
  * @brief   User Interface - LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  UI_LED_H
#define  UI_LED_H

#include	<stdbool.h>
#include	<string.h>
#include	"config.h"
#include	"bsp.h"

#define	UI_SYSTICK_HZ                   BSP_SYSTICK_HZ

#define	UI_LED_FLSH_SHRT_TCKS           ( ((uint32_t) CFG_UI_LED_FLSH_SHRT_mSEC * UI_SYSTICK_HZ) / 1000 )
#define	UI_LED_FLSH_LONG_TCKS           ( ((uint32_t) CFG_UI_LED_FLSH_LONG_mSEC * UI_SYSTICK_HZ) / 1000 )

/**
  * @brief   Single LED
  */

typedef    void    ( * ui_led_init_t )( void );
typedef    bool    ( * ui_led_get_t )( void );
typedef    void    ( * ui_led_set_t )( bool bval );

typedef	struct	ui_led_s
{
	ui_led_init_t           init;
	ui_led_set_t            set;
	ui_led_get_t            get;
	size_t                  tick;
} ui_led_t;


/**
  * @brief   Single Poll, Four Throw
  */
typedef enum    ui_led_gnss_mode_s
{
	//UI_LED_GNSS_STS_PASS,
	//UI_LED_GNSS_STS_SKIP,
	//UI_LED_GNSS_STS_OMIT,
	UI_LED_GNSS_MODE_NONE,
	UI_LED_GNSS_MODE_GPS,
	UI_LED_GNSS_MODE_DGPS,
	UI_LED_GNSS_MODE_RTKINT,
	UI_LED_GNSS_MODE_RTKFLT,
} ui_led_gnss_mode_t;

typedef	struct	ui_led_sp4t_s
{
	ui_led_gnss_mode_t      mode;
	ui_led_t                gps;
	ui_led_t                dgps;
	ui_led_t                rtkint;
	ui_led_t                rtkflt;
} ui_led_sp4t_t;


/**
  * @brief   Red, Green, Blue
  */
typedef enum    ui_led_rgb_color_s
{
	UI_LED_RGB_COLOR_BLACK,
	UI_LED_RGB_COLOR_RED,
	UI_LED_RGB_COLOR_GREEN,
	UI_LED_RGB_COLOR_BLUE,
	UI_LED_RGB_COLOR_YELLOW,
	UI_LED_RGB_COLOR_CYAN,
	UI_LED_RGB_COLOR_MAGENTA,
	UI_LED_RGB_COLOR_WHITE,
} ui_led_rgb_color_t;

typedef	struct	ui_led_rgb_s
{
	ui_led_rgb_color_t      color;
	ui_led_t                r;
	ui_led_t                g;
	ui_led_t                b;
} ui_led_rgb_t;


bool ui_led_hook(                       ui_led_t *              p );

void ui_led_toggle(                     ui_led_t *              p );

void ui_led_set(                        ui_led_t *              p,
                                const   bool                    mode );

void ui_led_flash(                      ui_led_t *              p,
                                        size_t                  len );


bool ui_led_sd_hook(                    void );

void ui_led_sd_toggle(                  void );

void ui_led_sd_set(             const   bool                    mode );

void ui_led_sd_flash(           const   size_t                  len );


bool ui_led_usb_hook(                   void );

void ui_led_usb_toggle(                 void );

void ui_led_usb_set(            const   bool                    mode );

void ui_led_usb_flash(          const   size_t                  len );


void ui_led_gnss_hook(                  void );

void ui_led_gnss_toggle(                void );

void ui_led_gnss_set(           const   ui_led_gnss_mode_t      mode );

void ui_led_gnss_flash(         const   size_t                  len );


void ui_led_pwr_hook(                   void );

void ui_led_pwr_toggle(                 void );

void ui_led_pwr_set(            const   ui_led_rgb_color_t      color );

void ui_led_pwr_flash(          const   size_t                  len );


#endif	//UI_LED_H
