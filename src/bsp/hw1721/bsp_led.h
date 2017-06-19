/**
  * @file    bsp_led.h
  * @brief   Board Support Package - LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_LED_H
#define  BSP_LED_H

#include	<stdbool.h>

typedef enum    bsp_led_color_e
{
        BSP_LED_COLOR_BLACK             =   0,
        BSP_LED_COLOR_RED               =   1,
        BSP_LED_COLOR_GREEN             =   2,
        BSP_LED_COLOR_YELLOW            =   3,
        BSP_LED_COLOR_BLUE              =   4,
        BSP_LED_COLOR_MAGENTA           =   5,
        BSP_LED_COLOR_CIAN              =   6,
        BSP_LED_COLOR_WHITE             =   7,
} bsp_led_color_t;

void            bsp_led_gnss_init( void );
bsp_led_color_t bsp_led_gnss_get( void );
void            bsp_led_gnss_set(       const   bsp_led_color_t         color );


void	        bsp_led_pwr_init( void );
bsp_led_color_t bsp_led_pwr_get( void );
void	        bsp_led_pwr_set(        const   bsp_led_color_t         color );


void            bsp_led_sd_init( void );
bsp_led_color_t bsp_led_sd_get( void );
void	        bsp_led_sd_set(         const   bsp_led_color_t         color );
//int  bsp_led_sd_get( void );
//void bsp_led_sd_set(                    const   int             bval );

void            bsp_led_usb_init( void );
bsp_led_color_t bsp_led_usb_get( void );
void            bsp_led_usb_set(        const   bsp_led_color_t         color );
//int  bsp_led_usb_get( void );
//void bsp_led_usb_set(                   const   int             bval );

#endif	//BSP_LED_H
