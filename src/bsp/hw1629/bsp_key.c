/**
  * @file    bsp_key.h
  * @brief   Board Support Package - Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_key.h"
#include	"bsp.h"

/**
 * @brief BSP key initialization.
 */
void	bsp_key0_init( void )
{
	GPIO_InitTypeDef        gpio_key0       =   {   .Pin    =   GPIO_PIN_0,
	                                                .Mode   =   GPIO_MODE_INPUT,
	                                                .Pull   =   GPIO_PULLDOWN,
	                                                .Speed  =   GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_Init(          GPIOA,          &gpio_key0   );
}

/**
 * @brief BSP key initialization.
 */
void	bsp_key1_init( void )
{
	GPIO_InitTypeDef        gpio_key1       =   {   .Pin    =   GPIO_PIN_8,
	                                                .Mode   =   GPIO_MODE_INPUT,
	                                                .Pull   =   GPIO_PULLDOWN,
	                                                .Speed  =   GPIO_SPEED_FREQ_LOW };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init(          GPIOB,          &gpio_key1   );
}

/**
 * @brief BSP key state get
 */
bool	bsp_key0_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOA, GPIO_PIN_0 ) ? true : false );
}

/**
 * @brief BSP key state get
 */
bool	bsp_key1_get( void )
{
	return( HAL_GPIO_ReadPin( GPIOB, GPIO_PIN_8 ) ? true : false );
}
