/**
  * @file    app_isr.c
  * @brief   App - Interrupt Service Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */

//#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <time.h>
#include "stm32f4xx.h"
#include "app_trace.h"
#include "ui.h"
#include "ff.h"
#include "gnss.h"
#include "flog.h"
#include "pmu.h"
#include "app.h"

//extern	ui_t                    ui;
extern	gnss_t                  gnss;
extern	app_t                   app;
extern	flog_t                  flog;
	time_t                  time_dat        =   0;
extern	UART_HandleTypeDef      huart;

extern	PCD_HandleTypeDef       hpcd;

void	hard_fault_handler( uint32_t *arg );
void	NMI_Handler( void );
void	HardFault_Handler( void );
void	MemManage_Handler( void );
void	BusFault_Handler( void );
void	UsageFault_Handler( void );
void	WWDG_IRQHandler( void );
void	SysTick_Handler( void );
void    EXTI0_IRQHandler( void );
void	USART1_IRQHandler( void );
void	DMA2_Stream3_IRQHandler( void );
void	DMA2_Stream6_IRQHandler( void );
void	SDIO_IRQHandler( void );

/**
 * @brief Hard Fault handler
 */
void hard_fault_handler(uint32_t *arg)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	volatile uint32_t stack_ptr;
	volatile uint32_t stacked_r0;
	volatile uint32_t stacked_r1;
	volatile uint32_t stacked_r2;
	volatile uint32_t stacked_r3;
	volatile uint32_t stacked_r12;
	volatile uint32_t stacked_lr;
	volatile uint32_t stacked_pc;
	volatile uint32_t stacked_psr;

	stack_ptr = (uint32_t)arg;
	stacked_r0 = arg[0];
	stacked_r1 = arg[1];
	stacked_r2 = arg[2];
	stacked_r3 = arg[3];
	stacked_r12 = arg[4];
	stacked_lr = arg[5];
	stacked_pc = arg[6];
	stacked_psr = arg[7];

	APP_TRACE("\r\n\n--- Hard fault handler ---\n");
	APP_TRACE("stck_ptr\t= 0x%08X\n", stack_ptr);
	APP_TRACE("R0\t= 0x%08X\n", stacked_r0);
	APP_TRACE("R1\t= 0x%08X\n", stacked_r1);
	APP_TRACE("R2\t= 0x%08X\n", stacked_r2);
	APP_TRACE("R3\t= 0x%08X\n", stacked_r3);
	APP_TRACE("R12\t= 0x%08X\n", stacked_r12);
	APP_TRACE("LR[R14]\t= 0x%08X\tsubroutine call return address\n", stacked_lr);
	APP_TRACE("PC[R15]\t= 0x%08X\tprogram counter\n", stacked_pc);
	APP_TRACE("PSR\t= 0x%08X\n", stacked_psr);
	APP_TRACE("BFAR\t= 0x%08Xh\n", (uint32_t)(*((volatile uint32_t *)(0xE000ED38))));
	APP_TRACE("CFSR\t= 0x%08Xh\n", (uint32_t)(*((volatile uint32_t *)(0xE000ED28))));
	APP_TRACE("HFSR\t= 0x%08Xh\n", (uint32_t)(*((volatile uint32_t *)(0xE000ED2C))));
	APP_TRACE("DFSR\t= 0x%08Xh\n", (uint32_t)(*((volatile uint32_t *)(0xE000ED30))));
	APP_TRACE("AFSR\t= 0x%08Xh\n", (uint32_t)(*((volatile uint32_t *)(0xE000ED3C))));
	APP_TRACE("SCB_SHCSR\t= 0x%08Xh\n", SCB->SHCSR);
	// memstat();
	__asm volatile ("bkpt #0\n");         // Break into the debugger
	while(1);
	#endif //NDEBUG
}

/**
 * @brief NMI interrupt handler
 */
void NMI_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief HardFault interrupt handler
 */
void HardFault_Handler(void)
{
	asm volatile
	(
	        "TST	LR,		#4			\n"
	        "ITE	EQ					\n"
	        "MRSEQ	R0,		MSP			\n"
	        "MRSNE	R0,		PSP			\n"
	        "B		hard_fault_handler"
	);
}

/**
 * @brief Memory Management interrupt handler
 */
void MemManage_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief Bus Fault interrupt handler
 */
void BusFault_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}

	while(1);
	#endif //NDEBUG
}

/**
 * @brief Usage Fault interrupt handler
 */
void UsageFault_Handler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief WindowWatchDog interrupt handler
 */
void WWDG_IRQHandler(void)
{
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	if (CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) {
		__asm volatile ("bkpt #0\n"); // Break into the debugger
	}
	while(1);
	#endif //NDEBUG
}

/**
 * @brief SysTick timer interrupt handler
 */
void SysTick_Handler(void)
{
	ui_led_sd_hook();
	ui_led_usb_hook();
	ui_led_pwr_hook();
	ui_led_gnss_hook();

	app.evt.ui_key0     =    ui_key_pwr_hook() ? true : false;
	app.evt.ui_key1     =    ui_key_func_hook() ? true : false;

	//app.evt.ui_key0     =    ui_key_hook( &ui.key[0] ) ? true : false;
	//app.evt.ui_key1     =    ui_key_hook( &ui.key[1] ) ? true : false;

	if( ++(app.tick_1hz) > BSP_SYSTICK_HZ )
	{
		app.tick_1hz        =    0;
		app.evt.tick_1hz    =    true;

		time_dat++;

		gnss_time_sync( &gnss, &time_dat );

		#ifndef	NDEBUG
		app.tick_1hz_cnt++;
		#endif
	}

	HAL_IncTick();
}

/**
 * @brief EXTI0 interrupt handler
 */
void    EXTI0_IRQHandler( void )
{
	if( __HAL_GPIO_EXTI_GET_IT( BSP_IRQ_EXTI_0 ) != RESET )
	{
		__HAL_GPIO_EXTI_CLEAR_IT( BSP_IRQ_EXTI_0 );
	}
}

/**
 * @brief EXTI1 interrupt handler
 */
void    EXTI1_IRQHandler( void )
{
	if( __HAL_GPIO_EXTI_GET_IT( BSP_IRQ_EXTI_1 ) != RESET )
	{
		__HAL_GPIO_EXTI_CLEAR_IT( BSP_IRQ_EXTI_1 );
	}
}

/**
 * @brief EXTI1 interrupt handler
 */
void    EXTI2_IRQHandler( void )
{
	if( __HAL_GPIO_EXTI_GET_IT( BSP_IRQ_EXTI_2 ) != RESET )
	{
		__HAL_GPIO_EXTI_CLEAR_IT( BSP_IRQ_EXTI_2 );

		if( flog.sts.enable )
		{
			app.evt.log_write   =   true;
			flog_write( &flog, flog.buf_full, CFG_FMNG_BLCK_SIZE_OCT );

		}
	}
}

/**
 * @brief USART1 interrupt handler
 */
void    USART1_IRQHandler( void )
{
	uint32_t        sts                     =   USART1->SR;
	uint32_t        data                    =   USART1->DR;

/*
	//uint32_t        data;
	bool            gnss_recv_complete;
	bool            fmng_buff_complete;


	if( sts & USART_SR_RXNE )
	{
		USART1->SR      =   0;
		USART1->CR1     |=  (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR3_EIE);
		//data                    =   USART1->DR;
		gnss_recv_complete	=   gnss_recv_hook( &gnss, data );
		fmng_buff_complete      =   flog_save_hook( &flog, data );

		if( gnss_recv_complete )
		{
			__HAL_GPIO_EXTI_GENERATE_SWIT( BSP_IRQ_EXTI_0 );
		}

		if( fmng_buff_complete )
		{
			__HAL_GPIO_EXTI_GENERATE_SWIT( BSP_IRQ_EXTI_1 );
		}
	}
*/
}

/**
  * @brief  DMA USART1 TX
  */
void DMA2_Stream7_IRQHandler( void )
{
	HAL_DMA_IRQHandler( huart.hdmatx );
}

/**
  * @brief  DMA USART1 RX
  */
void DMA2_Stream2_IRQHandler( void )
{
	HAL_DMA_IRQHandler( huart.hdmarx );
}

/**
  * @brief  This function handles SDIO interrupt request.
  * @param  None
  * @retval None
  */
void SDIO_IRQHandler(void)
{
	BSP_SD_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream 3 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream3_IRQHandler(void)
{
	BSP_SD_DMA_Rx_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream 6 interrupt request.
  * @param  None
  * @retval None
  */
void DMA2_Stream6_IRQHandler(void)
{
	BSP_SD_DMA_Tx_IRQHandler(); 
}

/**
  * @brief  This function handles USB-On-The-Go FS/HS global interrupt request.
  * @param  None
  * @retval None
  */
#ifdef USE_USB_FS
void OTG_FS_IRQHandler(void)
#else
void OTG_HS_IRQHandler(void)
#endif
{
	HAL_PCD_IRQHandler( &hpcd );
}
