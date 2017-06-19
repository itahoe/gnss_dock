/**
  * @file    bsp_led_gnss.h
  * @brief   Board Support Package - GNSS LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_led.h"
#include	"bsp.h"


/**
 * @brief GNSS LED pin initialization
 */
void	bsp_led_gnss_init( void )
{
	GPIO_InitTypeDef        gpio_led_r              =   {    .Pin       =    GPIO_PIN_10,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	GPIO_InitTypeDef        gpio_led_g              =   {    .Pin       =    GPIO_PIN_8,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	GPIO_InitTypeDef        gpio_led_b              =   {    .Pin       =    GPIO_PIN_9,
	                                                         .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                         .Pull      =    GPIO_NOPULL,
	                                                         .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_r );
	HAL_GPIO_Init( GPIOA, &gpio_led_g );
	HAL_GPIO_Init( GPIOA, &gpio_led_b );
}


/**
 * @brief LED0 get state
 */
bsp_led_color_t bsp_led_gnss_get( void )
{
        bool    r, g, b;
        bsp_led_color_t         color;

        r       =   HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_10 ) ? 0 : 1;
        g       =   HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_8  ) ? 0 : 1;
        b       =   HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_9  ) ? 0 : 1;

        color   =   (bsp_led_color_t) ( (r << 0) | (g << 1) | (b << 2) );

        return( color );
}

void bsp_led_gnss_set(                  const   bsp_led_color_t         color )
{

        bool                    r, g, b;

        switch( color )
        {
                case BSP_LED_COLOR_BLACK:       r = false;      g = false;      b = false;      break;
                case BSP_LED_COLOR_RED:         r = true;       g = false;      b = false;      break;
                case BSP_LED_COLOR_GREEN:       r = false;      g = true;       b = false;      break;
                case BSP_LED_COLOR_YELLOW:      r = true;       g = true;       b = false;      break;
                case BSP_LED_COLOR_BLUE:        r = false;      g = false;      b = true;       break;
                case BSP_LED_COLOR_MAGENTA:     r = true;       g = false;      b = true;       break;
                case BSP_LED_COLOR_CIAN:        r = false;      g = true;       b = true;       break;
                case BSP_LED_COLOR_WHITE:       r = true;       g = true;       b = true;       break;
                default:                        r = false;      g = false;      b = false;      break;
        }

	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_10,  r ? GPIO_PIN_RESET : GPIO_PIN_SET );
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8,   g ? GPIO_PIN_RESET : GPIO_PIN_SET );
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9,   b ? GPIO_PIN_RESET : GPIO_PIN_SET );

}

/*
bool	bsp_led_gnss_r_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_10 ) ? false : true );
}


bool	bsp_led_gnss_g_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_8 ) ? false : true );
}


bool	bsp_led_gnss_b_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_9 ) ? false : true );
}
*/

/*
void bsp_led_gnss_r_set(                const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_10,  bval ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

void bsp_led_gnss_g_set(                const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_8,  bval ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

void bsp_led_gnss_b_set(                const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_9,  bval ? GPIO_PIN_RESET : GPIO_PIN_SET );
}
*/
