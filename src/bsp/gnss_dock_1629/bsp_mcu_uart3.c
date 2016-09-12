/**
  * @file    bsp_mcu_uart3.c
  * @brief   BSP MCU UART3
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_mcu.h"
#include "bsp.h"


static  DMA_HandleTypeDef       hdma_tx;
static  DMA_HandleTypeDef       hdma_rx;
static  UART_HandleTypeDef      huart;


static
void bsp_mcu_uart_io_init( void )
{
	GPIO_InitTypeDef        gpio_tx     =   {    .Pin       =    GPIO_PIN_10,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART3 };

	GPIO_InitTypeDef        gpio_rx     =   {    .Pin       =    GPIO_PIN_5,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART3 };

	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();

	HAL_GPIO_Init( GPIOB, &gpio_tx );
	HAL_GPIO_Init( GPIOC, &gpio_rx );
}

static
int bsp_mcu_uart_dma_init( void )
{
	int                     resp    =   0;


	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma_tx.Instance                        =   DMA1_Stream3;
	hdma_tx.Init.Channel                    =   DMA_CHANNEL_4;
	hdma_tx.Init.Direction                  =   DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc                  =   DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc                     =   DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment        =   DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment           =   DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                       =   DMA_NORMAL;
	hdma_tx.Init.Priority                   =   DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode                   =   DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold              =   DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst                   =   DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst                =   DMA_PBURST_INC4;

	HAL_DMA_Init( &hdma_tx );
	__HAL_LINKDMA( &huart, hdmatx, hdma_tx );

	hdma_rx.Instance                        =   DMA1_Stream1;
	hdma_rx.Init.Channel                    =   DMA_CHANNEL_4;
	hdma_rx.Init.Direction                  =   DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc                  =   DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc                     =   DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment        =   DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment           =   DMA_MDATAALIGN_BYTE;
	hdma_rx.Init.Mode                       =   DMA_CIRCULAR;
	hdma_rx.Init.Priority                   =   DMA_PRIORITY_HIGH;
	hdma_rx.Init.FIFOMode                   =   DMA_FIFOMODE_DISABLE;
	hdma_rx.Init.FIFOThreshold              =   DMA_FIFO_THRESHOLD_FULL;
	hdma_rx.Init.MemBurst                   =   DMA_MBURST_INC4;
	hdma_rx.Init.PeriphBurst                =   DMA_PBURST_INC4;

	HAL_DMA_Init( &hdma_rx );
	__HAL_LINKDMA( &huart, hdmarx, hdma_rx );

	return( resp );
}

/**
 * @brief UART3
 */
void bsp_mcu_uart3_init(                const   size_t          baud )
{
	huart.Instance           =   USART3;
	huart.Init.BaudRate      =   baud;
	huart.Init.WordLength    =   UART_WORDLENGTH_8B;
	huart.Init.StopBits      =   UART_STOPBITS_1;
	huart.Init.Parity        =   UART_PARITY_NONE;
	huart.Init.Mode          =   UART_MODE_TX_RX;
	huart.Init.HwFlowCtl     =   UART_HWCONTROL_NONE;
	huart.Init.OverSampling  =   UART_OVERSAMPLING_16;


	__HAL_RCC_USART3_CLK_ENABLE();

	__HAL_RCC_USART3_FORCE_RESET();
	__HAL_RCC_USART3_RELEASE_RESET();

	HAL_UART_Init( &huart );

	bsp_mcu_uart_io_init();
	bsp_mcu_uart_dma_init();


	HAL_NVIC_SetPriority(   DMA1_Stream3_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_TX, 0 ); //DMA TX
	HAL_NVIC_EnableIRQ(     DMA1_Stream3_IRQn );

	HAL_NVIC_SetPriority(   DMA1_Stream1_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_RX, 0 ); //DMA RX
	HAL_NVIC_EnableIRQ(     DMA1_Stream1_IRQn );

	HAL_NVIC_SetPriority(   USART3_IRQn,            BSP_NVIC_PRIO_GNSS_RECV_SMBL, 0 );
	HAL_NVIC_EnableIRQ( USART3_IRQn );
}

/**
 * @brief UART3
 */
void bsp_mcu_uart3_isr(                         void )
{
        HAL_UART_IRQHandler( &huart );
}

/**
 * @brief UART3
 */
void bsp_mcu_uart3_dma_tx_isr(                  void )
{
        HAL_DMA_IRQHandler( huart.hdmatx );
}

/**
 * @brief UART3
 */
void bsp_mcu_uart3_dma_rx_isr(                  void )
{
        HAL_DMA_IRQHandler( huart.hdmarx );
}

/**
 * @brief UART3
 */
void bsp_mcu_uart3_xmit_start(              uint8_t *           data,
                                            size_t              size )
{
	HAL_UART_Transmit_DMA( &huart, data, size );
}

/**
 * @brief UART3
 */
void bsp_mcu_uart3_recv_start(              uint8_t *           data,
                                            size_t              size )
{
	HAL_UART_Receive_DMA( &huart, data, size );
}

/**
 * @brief UART3
 */
uint32_t bsp_mcu_uart3_recv_dma_get(           void )
{
	return( DMA1_Stream1->NDTR );
}
