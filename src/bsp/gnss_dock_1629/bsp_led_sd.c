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

void	bsp_led_sd_init( void )
{
	GPIO_InitTypeDef        gpio_led_sd     =   {    .Pin       =    GPIO_PIN_5,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_sd );
}


/*
void	bsp_led_sd_init( void )
{
	GPIO_InitTypeDef        gpio_led_sd     =   {    .Pin       =    GPIO_PIN_3,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_Init( GPIOC, &gpio_led_sd );
	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_3, GPIO_PIN_SET );

}
*/

/**
 * @brief LED0 state get.
 */
bool	bsp_led_sd_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_5 ) ? true : false );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_sd_set(                 const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_5,  bval ? GPIO_PIN_SET : GPIO_PIN_RESET );
}
