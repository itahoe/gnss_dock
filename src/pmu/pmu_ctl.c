/**
  * @file    pmu_ctl.h
  * @brief   Power Management Unit - Control Service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_pmu.h"
#include	"pmu.h"
#include	"bsp.h"

static
void	pmu_power_off( void )
{
	GPIO_InitTypeDef GPIO_InitStruct;


	__disable_irq();

	HAL_SuspendTick();

	pmu_ctl( PMU_CTL_LDO, false );

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();

	GPIO_InitStruct.Mode    =   GPIO_MODE_ANALOG;
	GPIO_InitStruct.Speed   =   GPIO_SPEED_HIGH;
	GPIO_InitStruct.Pull    =   GPIO_NOPULL;
	GPIO_InitStruct.Pin     =   GPIO_PIN_All;

	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOH, &GPIO_InitStruct); 
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

/*
	__HAL_GPIO_EXTI_CLEAR_IT( BSP_IRQ_EXTI_2 );

	GPIO_InitStruct.Pin     =   GPIO_PIN_2;
	GPIO_InitStruct.Pull    =   GPIO_PULLDOWN;
	GPIO_InitStruct.Mode    =   GPIO_MODE_IT_RISING; 
	HAL_GPIO_Init( GPIOB, &GPIO_InitStruct );

	HAL_NVIC_SetPriority( EXTI2_IRQn, 0x00, 0 );
	HAL_NVIC_EnableIRQ( EXTI2_IRQn );
*/

	__HAL_GPIO_EXTI_CLEAR_IT( BSP_IRQ_EXTI_0 );

	GPIO_InitStruct.Pin     =   GPIO_PIN_0;
	GPIO_InitStruct.Pull    =   GPIO_PULLDOWN;
	GPIO_InitStruct.Mode    =   GPIO_MODE_IT_RISING; 
	HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

	HAL_NVIC_SetPriority( EXTI0_IRQn, 0x00, 0 );
	HAL_NVIC_EnableIRQ( EXTI0_IRQn );

	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOD_CLK_DISABLE();
	__HAL_RCC_GPIOE_CLK_DISABLE();
	__HAL_RCC_GPIOH_CLK_DISABLE();


	//HAL_PWR_EnterSLEEPMode( PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI );

	HAL_PWREx_EnableFlashPowerDown(); //FLASH Deep Power Down Mode enabled
	HAL_PWR_EnterSTOPMode( PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI ); //Enter Stop Mode

	NVIC_SystemReset();
}


/**
 * @brief PMU Initialization.
 */
void	pmu_ctl(                    const   pmu_ctl_t           ctl,
                                    const   bool                bval )
{
	switch( ctl )
	{
		case PMU_CTL_LDO:       bsp_pmu_ldo_enable( bval );     break;
		case PMU_CTL_MCU_OFF:   pmu_power_off();                break;
		default:                                                break;
	}
}
