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
void	bsp_led_pwr_r_init( void )
{
	GPIO_InitTypeDef        gpio_led_pwr_r  =   {    .Pin       =    GPIO_PIN_4,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_Init( GPIOC, &gpio_led_pwr_r );
}

/**
 * @brief LED0 pin initialization
 */
void	bsp_led_pwr_g_init( void )
{
	GPIO_InitTypeDef        gpio_led_pwr_g  =   {    .Pin       =    GPIO_PIN_7,
	                                                 .Mode      =    GPIO_MODE_OUTPUT_PP,
	                                                 .Pull      =    GPIO_NOPULL,
	                                                 .Speed     =    GPIO_SPEED_FREQ_LOW };
	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_led_pwr_g );
}

void	bsp_led_pwr_b_init( void )
{
}

/**
 * @brief LED0 state get.
 */
bool	bsp_led_pwr_r_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOC, GPIO_PIN_4 ) ? false : true );
}

/**
 * @brief LED0 state get.
 */
bool	bsp_led_pwr_g_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_7 ) ? false : true );
}

/**
 * @brief LED0 state get.
 */
bool	bsp_led_pwr_b_get( void )
{
	return( false );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_pwr_r_set(              const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOC, GPIO_PIN_4,  bval ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_pwr_g_set(              const   bool                bval )
{
	HAL_GPIO_WritePin( GPIOA, GPIO_PIN_7,  bval ? GPIO_PIN_RESET : GPIO_PIN_SET );
}

/**
 * @brief LED0 pin set
 */
void	bsp_led_pwr_b_set(              const   bool                bval )
{
}
