/**
  * @file    bsp_mcu_uart2.c
  * @brief   BSP MCU UART1
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_mcu.h"
#include "bsp.h"


static  DMA_HandleTypeDef       huart2_dma_tx;
static  DMA_HandleTypeDef       huart2_dma_rx;
static  UART_HandleTypeDef      huart2;


static
void bsp_mcu_uart2_io_init( void )
{
	GPIO_InitTypeDef        gpio_tx     =   {    .Pin       =    GPIO_PIN_2,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART2 };

	GPIO_InitTypeDef        gpio_rx     =   {    .Pin       =    GPIO_PIN_3,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART2 };

	__HAL_RCC_GPIOA_CLK_ENABLE();

	HAL_GPIO_Init( GPIOA, &gpio_rx );
	HAL_GPIO_Init( GPIOA, &gpio_tx );
}

static
int bsp_mcu_uart2_dma_init( void )
{
	int                     resp    =   0;


	__HAL_RCC_DMA1_CLK_ENABLE();

	huart2_dma_tx.Instance                  =   DMA1_Stream6;
	huart2_dma_tx.Init.Channel              =   DMA_CHANNEL_4;
	huart2_dma_tx.Init.Direction            =   DMA_MEMORY_TO_PERIPH;
	huart2_dma_tx.Init.PeriphInc            =   DMA_PINC_DISABLE;
	huart2_dma_tx.Init.MemInc               =   DMA_MINC_ENABLE;
	huart2_dma_tx.Init.PeriphDataAlignment  =   DMA_PDATAALIGN_BYTE;
	huart2_dma_tx.Init.MemDataAlignment     =   DMA_MDATAALIGN_BYTE;
	huart2_dma_tx.Init.Mode                 =   DMA_NORMAL;
	huart2_dma_tx.Init.Priority             =   DMA_PRIORITY_LOW;
	huart2_dma_tx.Init.FIFOMode             =   DMA_FIFOMODE_DISABLE;
	huart2_dma_tx.Init.FIFOThreshold        =   DMA_FIFO_THRESHOLD_FULL;
	huart2_dma_tx.Init.MemBurst             =   DMA_MBURST_INC4;
	huart2_dma_tx.Init.PeriphBurst          =   DMA_PBURST_INC4;

	HAL_DMA_Init( &huart2_dma_tx );
	__HAL_LINKDMA( &huart2, hdmatx, huart2_dma_tx );

	huart2_dma_rx.Instance                  =   DMA1_Stream5;
	huart2_dma_rx.Init.Channel              =   DMA_CHANNEL_4;
	huart2_dma_rx.Init.Direction            =   DMA_PERIPH_TO_MEMORY;
	huart2_dma_rx.Init.PeriphInc            =   DMA_PINC_DISABLE;
	huart2_dma_rx.Init.MemInc               =   DMA_MINC_ENABLE;
	huart2_dma_rx.Init.PeriphDataAlignment  =   DMA_PDATAALIGN_BYTE;
	huart2_dma_rx.Init.MemDataAlignment     =   DMA_MDATAALIGN_BYTE;
	huart2_dma_rx.Init.Mode                 =   DMA_CIRCULAR;
	huart2_dma_rx.Init.Priority             =   DMA_PRIORITY_HIGH;
	huart2_dma_rx.Init.FIFOMode             =   DMA_FIFOMODE_DISABLE;
	huart2_dma_rx.Init.FIFOThreshold        =   DMA_FIFO_THRESHOLD_FULL;
	huart2_dma_rx.Init.MemBurst             =   DMA_MBURST_INC4;
	huart2_dma_rx.Init.PeriphBurst          =   DMA_PBURST_INC4;

	HAL_DMA_Init( &huart2_dma_rx );
	__HAL_LINKDMA( &huart2, hdmarx, huart2_dma_rx );

	return( resp );
}

/**
 * @brief
 */
void bsp_mcu_uart2_init(                const   size_t                  baud )
{
        huart2.Instance                         =   USART2;
        huart2.Init.BaudRate                    =   baud;
        huart2.Init.WordLength                  =   UART_WORDLENGTH_8B;
        huart2.Init.StopBits                    =   UART_STOPBITS_1;
        huart2.Init.Parity                      =   UART_PARITY_NONE;
        huart2.Init.Mode                        =   UART_MODE_TX_RX;
        huart2.Init.HwFlowCtl                   =   UART_HWCONTROL_NONE;
        huart2.Init.OverSampling                =   UART_OVERSAMPLING_16;

        __HAL_RCC_USART2_CLK_ENABLE();
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        HAL_UART_Init( &huart2 );

        bsp_mcu_uart2_io_init();
        bsp_mcu_uart2_dma_init();


	   HAL_NVIC_SetPriority(   DMA1_Stream6_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_TX, 0 ); //DMA TX
	   HAL_NVIC_EnableIRQ(     DMA1_Stream6_IRQn );

	   HAL_NVIC_SetPriority(   DMA1_Stream5_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_RX, 0 ); //DMA RX
	   HAL_NVIC_EnableIRQ(     DMA1_Stream5_IRQn );

	   HAL_NVIC_SetPriority(   USART2_IRQn,            BSP_NVIC_PRIO_GNSS_RECV_SMBL, 0 );
	   HAL_NVIC_EnableIRQ( USART2_IRQn );
}

/**
 * @brief UART1
 */
void bsp_mcu_uart2_isr(                         void )
{
        HAL_UART_IRQHandler( &huart2 );
}

/**
 * @brief UART1
 */
void bsp_mcu_uart2_recv_start(              uint8_t *           data,
                                            size_t              size )
{
        HAL_UART_Receive_DMA( &huart2, data, size );

        SET_BIT( huart2.Instance->CR1,  USART_CR1_IDLEIE        );
}

/**
 * @brief UART1
 */
void bsp_mcu_uart2_dma_tx_isr(                  void )
{
        HAL_DMA_IRQHandler( huart2.hdmatx );
}

/**
 * @brief UART1
 */
void bsp_mcu_uart2_dma_rx_isr(                  void )
{
        HAL_DMA_IRQHandler( huart2.hdmarx );
}

/**
 * @brief UART1
 */
void bsp_mcu_uart2_xmit_start(                  uint8_t *               data,
                                                size_t                  size )
{
        HAL_UART_Transmit_DMA( &huart2, data, size );
}

/**
 * @brief UART1
 */
uint32_t bsp_mcu_uart2_recv_dma_head_get(       void )
{
        //return( DMA2_Stream2->NDTR );
        return( huart2.hdmarx->Instance->NDTR );
}

/**
 * @brief UART1
 */
/*
bool    bsp_mcu_uart1_recv_dma_full_get(       void )
{
        bool    resp    =   (DMA2->LISR & DMA_LISR_TCIF2) ? true : false;

        if( resp )
        {
                DMA2->LIFCR     |=   DMA_LIFCR_CTCIF2;
        }

        return(  resp );
}
*/
bool    bsp_mcu_uart2_sts_idle(                 void )
{
        bool    sts_idle        =   READ_BIT( huart2.Instance->SR, USART_SR_IDLE );

        return( sts_idle );
}
