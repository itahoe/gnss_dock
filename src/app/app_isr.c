/**
  * @file    app_isr.c
  * @brief   App - Interrupt Service Routines
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include "stm32f4xx.h"
#include "ui.h"
#include "bsp_mcu.h"
#include "app.h"
#include "cmsis_os.h"


extern  PCD_HandleTypeDef       hpcd;


void hard_fault_handler( uint32_t *arg );
void NMI_Handler( void );
void HardFault_Handler( void );
void MemManage_Handler( void );
void BusFault_Handler( void );
void UsageFault_Handler( void );
void WWDG_IRQHandler( void );
void SysTick_Handler( void );
void EXTI0_IRQHandler( void );
void USART1_IRQHandler( void );
void DMA2_Stream3_IRQHandler( void );
void DMA2_Stream6_IRQHandler( void );
void SDIO_IRQHandler( void );

void DMA2_Stream7_IRQHandler( void );
void DMA2_Stream2_IRQHandler( void );
void DMA1_Stream6_IRQHandler( void );
void DMA1_Stream5_IRQHandler( void );
void DMA1_Stream3_IRQHandler( void );
void DMA1_Stream1_IRQHandler( void );
void USART2_IRQHandler( void );
void USART3_IRQHandler( void );
void OTG_FS_IRQHandler( void );


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

	APP_TRACE("\r\n--- Hard fault handler ---\n");
	APP_TRACE("\nstck_ptr\t= 0x%08X", stack_ptr);
	APP_TRACE("\nR0\t= 0x%08X", stacked_r0);
	APP_TRACE("\nR1\t= 0x%08X", stacked_r1);
	APP_TRACE("\nR2\t= 0x%08X", stacked_r2);
	APP_TRACE("\nR3\t= 0x%08X", stacked_r3);
	APP_TRACE("\nR12\t= 0x%08X", stacked_r12);
	APP_TRACE("\nLR[R14]\t= 0x%08X\tsubroutine call return address", stacked_lr);
	APP_TRACE("\nPC[R15]\t= 0x%08X\tprogram counter", stacked_pc);
	APP_TRACE("\nPSR\t= 0x%08X", stacked_psr);
	APP_TRACE("\nBFAR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED38))));
	APP_TRACE("\nCFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED28))));
	APP_TRACE("\nHFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED2C))));
	APP_TRACE("\nDFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED30))));
	APP_TRACE("\nAFSR\t= 0x%08X", (uint32_t)(*((volatile uint32_t *)(0xE000ED3C))));
	APP_TRACE("\nSCB_SHCSR\t= 0x%08X", SCB->SHCSR);
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
	#ifdef  NDEBUG
	NVIC_SystemReset();
	#else
	asm volatile
	(
	        "TST	LR,		#4			\n"
	        "ITE	EQ					\n"
	        "MRSEQ	R0,		MSP			\n"
	        "MRSNE	R0,		PSP			\n"
	        "B		hard_fault_handler"
	);
	#endif //NDEBUG

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
    osSystickHandler();
}


/** @brief USART1
 */
void USART1_IRQHandler( void )
{
        bsp_mcu_uart1_isr();
}


/** @brief USART1 DMA TX
  */
void DMA2_Stream7_IRQHandler( void )
{
        bsp_mcu_uart1_dma_tx_isr();
}


/** @brief USART1 DMA RX
  */
void DMA2_Stream2_IRQHandler( void )
{
        bsp_mcu_uart1_dma_rx_isr();
}


/** @brief USART2
 */
void USART2_IRQHandler( void )
{
        bsp_mcu_uart2_isr();
}


/** @brief USART2 DMA TX
  */
void DMA1_Stream6_IRQHandler( void )
{
        bsp_mcu_uart2_dma_tx_isr();
}


/** @brief USART2 DMA RX
  */
void DMA1_Stream5_IRQHandler( void )
{
        bsp_mcu_uart2_dma_rx_isr();
}


/** @brief USART3
 */
void USART3_IRQHandler( void )
{
        bsp_mcu_uart3_isr();
}


/** @brief USART3 DMA TX
  */
void DMA1_Stream3_IRQHandler( void )
{
        bsp_mcu_uart3_dma_tx_isr();
}


/** @brief USART3 DMA RX
  */
void DMA1_Stream1_IRQHandler( void )
{
        bsp_mcu_uart3_dma_rx_isr();
}


void HAL_UART_RxHalfCpltCallback(               UART_HandleTypeDef *    huart )
{
        if(      huart->Instance == USART1 )
        {
                app_ser1_recv_half_isr();
        }
        else if( huart->Instance == USART2 )
        {
                app_ser2_recv_half_isr();
        }
        else if( huart->Instance == USART3 )
        {
        }
}

void HAL_UART_RxCpltCallback(                   UART_HandleTypeDef *    huart )
{
        if(      huart->Instance == USART1 )
        {
                app_ser1_recv_full_isr();
        }
        else if( huart->Instance == USART2 )
        {
                app_ser2_recv_full_isr();
        }
        else if( huart->Instance == USART3 )
        {
                app_ser3_recv_full_isr();
        }
}


void HAL_UART_TxCpltCallback(                   UART_HandleTypeDef *    huart )
{
        if(      huart->Instance == USART1 )
        {
                app_ser1_xmit_full_isr();
        }
        else if( huart->Instance == USART2 )
        {
                app_ser2_xmit_full_isr();
        }
        else if( huart->Instance == USART3 )
        {
                app_ser3_xmit_full_isr();
        }
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

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
/*
void TIM3_IRQHandler( void )
{
        HAL_TIM_IRQHandler( &htim_cdc );
}
*/
