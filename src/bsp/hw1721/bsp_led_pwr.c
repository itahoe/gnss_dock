/**
  * @file    bsp_led.h
  * @brief   Board Support Package - LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_led.h"
#include	"bsp.h"


/**
 * @brief LED PWR pin's initialization
 */
void bsp_led_pwr_init( void )
{
	GPIO_InitTypeDef        gpio_led_pwr_r  =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	GPIO_InitTypeDef        gpio_led_pwr_g  =   {    .Pin       =    GPIO_PIN_7,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };


	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOC, &gpio_led_pwr_r );
	HAL_GPIO_Init( GPIOA, &gpio_led_pwr_g );
}


/**
 * @brief LED PWR state get.
 */
bsp_led_color_t bsp_led_pwr_get( void )
{
        bsp_led_color_t         color   =   BSP_LED_COLOR_BLACK;
        int                     r, g;

	r       =   HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_4 ) ? 0 : 1;
	g       =   HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_7 ) ? 0 : 1;
        color   =   (bsp_led_color_t) ( (r << 0) | (g << 1) );

        return( color );
}


/**
 * @brief LED PWR set
 */
void bsp_led_pwr_set(                   const   bsp_led_color_t         color )
{
        bool                    r, g;

        switch( color )
        {
                case BSP_LED_COLOR_BLACK:       r = false;      g = false;      break;
                case BSP_LED_COLOR_RED:         r = true;       g = false;      break;
                case BSP_LED_COLOR_GREEN:       r = false;      g = true;       break;
                case BSP_LED_COLOR_YELLOW:      r = true;       g = true;       break;
                case BSP_LED_COLOR_BLUE:        r = false;      g = false;      break;
                case BSP_LED_COLOR_MAGENTA:     r = true;       g = false;      break;
                case BSP_LED_COLOR_CIAN:        r = false;      g = true;       break;
                case BSP_LED_COLOR_WHITE:       r = true;       g = true;       break;
                default:                        r = false;      g = false;      break;
        }

	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_4,  r ? GPIO_PIN_RESET : GPIO_PIN_SET );
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7,  g ? GPIO_PIN_RESET : GPIO_PIN_SET );
}
