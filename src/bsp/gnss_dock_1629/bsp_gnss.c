/**
  * @file    bsp_gnss.c
  * @brief   Board Support Package - GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<string.h>
#include	"bsp_gnss.h"
#include	"bsp.h"
#include	"config.h"


static	DMA_HandleTypeDef       hdma_tx;
static	DMA_HandleTypeDef       hdma_rx;
	UART_HandleTypeDef      huart;


static
void	bsp_gnss_init_uart_io( void )
{
	GPIO_InitTypeDef        gpio_rx     =   {    .Pin       =    GPIO_PIN_7,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };

	GPIO_InitTypeDef        gpio_tx     =   {    .Pin       =    GPIO_PIN_6,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };

	__HAL_RCC_GPIOB_CLK_ENABLE();

	HAL_GPIO_Init( GPIOB, &gpio_rx );
	HAL_GPIO_Init( GPIOB, &gpio_tx );
}

static
int     bsp_gnss_init_uart_dma( void )
{
	int                     resp    =   0;


	__HAL_RCC_DMA2_CLK_ENABLE();


	hdma_tx.Instance                        =   DMA2_Stream7;
	hdma_tx.Init.Channel                    =   DMA_CHANNEL_4;
	hdma_tx.Init.Direction                  =   DMA_MEMORY_TO_PERIPH;
	hdma_tx.Init.PeriphInc                  =   DMA_PINC_DISABLE;
	hdma_tx.Init.MemInc                     =   DMA_MINC_ENABLE;
	hdma_tx.Init.PeriphDataAlignment        =   DMA_PDATAALIGN_BYTE;
	hdma_tx.Init.MemDataAlignment           =   DMA_MDATAALIGN_BYTE;
	hdma_tx.Init.Mode                       =   DMA_CIRCULAR;
	hdma_tx.Init.Priority                   =   DMA_PRIORITY_LOW;
	hdma_tx.Init.FIFOMode                   =   DMA_FIFOMODE_DISABLE;
	hdma_tx.Init.FIFOThreshold              =   DMA_FIFO_THRESHOLD_FULL;
	hdma_tx.Init.MemBurst                   =   DMA_MBURST_INC4;
	hdma_tx.Init.PeriphBurst                =   DMA_PBURST_INC4;

	HAL_DMA_Init( &hdma_tx );
	__HAL_LINKDMA( &huart, hdmatx, hdma_tx );


	hdma_rx.Instance                        =   DMA2_Stream2;
	hdma_rx.Init.Channel                    =   DMA_CHANNEL_4;
	hdma_rx.Init.Direction                  =   DMA_PERIPH_TO_MEMORY;
	hdma_rx.Init.PeriphInc                  =   DMA_PINC_DISABLE;
	hdma_rx.Init.MemInc                     =   DMA_MINC_ENABLE;
	hdma_rx.Init.PeriphDataAlignment        =   DMA_PDATAALIGN_BYTE;
	hdma_rx.Init.MemDataAlignment           =   DMA_MDATAALIGN_BYTE;
	//hdma_rx.Init.Mode                       =   DMA_NORMAL;
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

static
int     bsp_gnss_init_uart( void )
{
	int                     resp    =   0;

	huart.Instance           =   USART1;
	huart.Init.BaudRate      =   CFG_NMEA_UART_BAUDRATE;
	huart.Init.WordLength    =   UART_WORDLENGTH_8B;
	huart.Init.StopBits      =   UART_STOPBITS_1;
	huart.Init.Parity        =   UART_PARITY_NONE;
	huart.Init.Mode          =   UART_MODE_TX_RX;
	huart.Init.HwFlowCtl     =   UART_HWCONTROL_NONE;
	huart.Init.OverSampling  =   UART_OVERSAMPLING_16;


	__HAL_RCC_USART1_CLK_ENABLE();

	__HAL_RCC_USART1_FORCE_RESET();
	__HAL_RCC_USART1_RELEASE_RESET();

	if( HAL_UART_Init( &huart ) != HAL_OK )
	{
		resp    =   -1;
	}

	return( resp );
}

/**
 * @brief BSP GNSS initialization
 */
void	bsp_gnss_init( void )
{
	bsp_gnss_init_uart_io();
	bsp_gnss_init_uart_dma();
	bsp_gnss_init_uart();

/*
	USART1->CR1     |=  (USART_CR1_RXNEIE | USART_CR1_PEIE | USART_CR3_EIE);
*/
	HAL_NVIC_SetPriority(   DMA2_Stream7_IRQn, 11, 0 ); //DMA TX
	HAL_NVIC_EnableIRQ(     DMA2_Stream7_IRQn );

	HAL_NVIC_SetPriority(   DMA2_Stream2_IRQn, 11, 0 ); //DMA RX
	HAL_NVIC_EnableIRQ(     DMA2_Stream2_IRQn );

	//HAL_NVIC_SetPriority( USART1_IRQn, 1, 0 );
	HAL_NVIC_SetPriority( USART1_IRQn, BSP_NVIC_PRIO_GNSS_RECV_SMBL, 0 );
	HAL_NVIC_EnableIRQ( USART1_IRQn );

	EXTI->IMR       |=  BSP_IRQ_EXTI_0;
	HAL_NVIC_SetPriority( EXTI0_IRQn, BSP_NVIC_PRIO_GNSS_RECV_STR, 0 );
	HAL_NVIC_EnableIRQ( EXTI0_IRQn );
}

/**
 * @brief BSP GNSS xmit block
 */
void bsp_gnss_xmit(                 const   char *              data,
                                            size_t              size )
{
	while( size-- )
	{
		while( !(USART1->SR & UART_FLAG_TXE) );
		USART1->DR      =   *data++;
	}
}

/**
 * @brief BSP GNSS recieve enable
 */
void bsp_gnss_recv_start(                   uint8_t *           data,
                                            size_t              size )
{
	HAL_UART_Receive_DMA( &huart, data, size );
}

/**
 * @brief BSP GNSS recieve disable
 */
void bsp_gnss_recv_stop( void )
{

}

/**
 * @brief BSP GNSS xmit enable
 */
void bsp_gnss_xmit_start(                   uint8_t *           data,
                                            size_t              size )
{
	HAL_UART_Transmit_DMA( &huart, data, size );
}

/**
 * @brief BSP GNSS xmit disable
 */
void bsp_gnss_xmit_stop( void )
{

}

uint32_t bsp_gnss_recv_fifo_head_get( void )
{
	return( DMA2_Stream2->M0AR );
}

uint32_t bsp_gnss_recv_fifo_size_get( void )
{
	return( DMA2_Stream2->NDTR );
}
