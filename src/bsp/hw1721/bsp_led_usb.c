/**
  * @file    bsp_led.h
  * @brief   Board Support Package - LED services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_led.h"
#include	"bsp.h"

/**
 * @brief LED0 pin initialization
 */
void	bsp_led_usb_init( void )
{
	GPIO_InitTypeDef        gpio_led_usb    =   {    .Pin       =    GPIO_PIN_1,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_usb );
}

/**
 * @brief LED0 state get.
 */
bsp_led_color_t         bsp_led_usb_get( void )
{
        bsp_led_color_t         color ;


        if( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_1 ) )
        {
                color   =   BSP_LED_COLOR_BLACK;
        }
        else
        {
                color   =   BSP_LED_COLOR_WHITE;
        }

	return( color );

}

/**
 * @brief LED0 pin set
 */
void bsp_led_usb_set(                const   bsp_led_color_t         color )
{
        if( color == BSP_LED_COLOR_BLACK )
        {
                HAL_GPIO_WritePin( GPIOA, GPIO_PIN_1,  GPIO_PIN_SET );
        }
        else
        {
                HAL_GPIO_WritePin( GPIOA, GPIO_PIN_1,  GPIO_PIN_RESET );
        }
}
