/**
  * @file    bsp_mcu_uart1.c
  * @brief   BSP MCU UART1
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_mcu.h"
#include "bsp.h"


static  DMA_HandleTypeDef       huart1_dma_tx;
static  DMA_HandleTypeDef       huart1_dma_rx;
static  UART_HandleTypeDef      huart1;


static
void bsp_mcu_uart1_io_init( void )
{
        GPIO_InitTypeDef        gpio_tx     =   {    .Pin       =    GPIO_PIN_6,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };

        GPIO_InitTypeDef        gpio_rx     =   {    .Pin       =    GPIO_PIN_7,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART1 };

        __HAL_RCC_GPIOB_CLK_ENABLE();

        HAL_GPIO_Init( GPIOB, &gpio_tx );
        HAL_GPIO_Init( GPIOB, &gpio_rx );
}


static
int bsp_mcu_uart1_dma_init( void )
{
        int                     resp    =   0;


        __HAL_RCC_DMA2_CLK_ENABLE();

        huart1_dma_tx.Instance                  =   DMA2_Stream7;
        huart1_dma_tx.Init.Channel              =   DMA_CHANNEL_4;
        huart1_dma_tx.Init.Direction            =   DMA_MEMORY_TO_PERIPH;
        huart1_dma_tx.Init.PeriphInc            =   DMA_PINC_DISABLE;
        huart1_dma_tx.Init.MemInc               =   DMA_MINC_ENABLE;
        huart1_dma_tx.Init.PeriphDataAlignment  =   DMA_PDATAALIGN_BYTE;
        huart1_dma_tx.Init.MemDataAlignment     =   DMA_MDATAALIGN_BYTE;
        huart1_dma_tx.Init.Mode                 =   DMA_NORMAL;
        huart1_dma_tx.Init.Priority             =   DMA_PRIORITY_LOW;
        huart1_dma_tx.Init.FIFOMode             =   DMA_FIFOMODE_DISABLE;
        huart1_dma_tx.Init.FIFOThreshold        =   DMA_FIFO_THRESHOLD_FULL;
        huart1_dma_tx.Init.MemBurst             =   DMA_MBURST_INC4;
        huart1_dma_tx.Init.PeriphBurst          =   DMA_PBURST_INC4;

        HAL_DMA_Init( &huart1_dma_tx );
        __HAL_LINKDMA( &huart1, hdmatx, huart1_dma_tx );

        huart1_dma_rx.Instance                  =   DMA2_Stream2;
        huart1_dma_rx.Init.Channel              =   DMA_CHANNEL_4;
        huart1_dma_rx.Init.Direction            =   DMA_PERIPH_TO_MEMORY;
        huart1_dma_rx.Init.PeriphInc            =   DMA_PINC_DISABLE;
        huart1_dma_rx.Init.MemInc               =   DMA_MINC_ENABLE;
        huart1_dma_rx.Init.PeriphDataAlignment  =   DMA_PDATAALIGN_BYTE;
        huart1_dma_rx.Init.MemDataAlignment     =   DMA_MDATAALIGN_BYTE;
        huart1_dma_rx.Init.Mode                 =   DMA_CIRCULAR;
        huart1_dma_rx.Init.Priority             =   DMA_PRIORITY_HIGH;
        huart1_dma_rx.Init.FIFOMode             =   DMA_FIFOMODE_DISABLE;
        huart1_dma_rx.Init.FIFOThreshold        =   DMA_FIFO_THRESHOLD_FULL;
        huart1_dma_rx.Init.MemBurst             =   DMA_MBURST_INC4;
        huart1_dma_rx.Init.PeriphBurst          =   DMA_PBURST_INC4;

        HAL_DMA_Init( &huart1_dma_rx );
        __HAL_LINKDMA( &huart1, hdmarx, huart1_dma_rx );

        return( resp );
}


bool bsp_mcu_uart1_init(                const   size_t                  baud )
{
        bool        resp    =   false;

        huart1.Instance                         =   USART1;
        huart1.Init.BaudRate                    =   baud;
        huart1.Init.WordLength                  =   UART_WORDLENGTH_8B;
        huart1.Init.StopBits                    =   UART_STOPBITS_1;
        huart1.Init.Parity                      =   UART_PARITY_NONE;
        huart1.Init.Mode                        =   UART_MODE_TX_RX;
        huart1.Init.HwFlowCtl                   =   UART_HWCONTROL_NONE;
        huart1.Init.OverSampling                =   UART_OVERSAMPLING_16;

        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        HAL_UART_Init( &huart1 );

        bsp_mcu_uart1_io_init();
        bsp_mcu_uart1_dma_init();

        HAL_NVIC_SetPriority(   DMA2_Stream7_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_TX, 0 );
        HAL_NVIC_EnableIRQ(     DMA2_Stream7_IRQn );

        HAL_NVIC_SetPriority(   DMA2_Stream2_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_RX, 0 );
        HAL_NVIC_EnableIRQ(     DMA2_Stream2_IRQn );

        HAL_NVIC_SetPriority(   USART1_IRQn,            BSP_NVIC_PRIO_GNSS_RECV_SMBL, 0 );
        HAL_NVIC_EnableIRQ( USART1_IRQn );

        return( resp );
}


void bsp_mcu_uart1_isr(                         void )
{
        HAL_UART_IRQHandler( &huart1 );
}


bool bsp_mcu_uart1_recv_start(              uint8_t *           data,
                                            size_t              size )
{
        bool        resp    =   false;

        HAL_UART_Receive_DMA( &huart1, data, size );

        //SET_BIT( huart1.Instance->CR1,  USART_CR1_IDLEIE        );

        return( resp );
}


bool bsp_mcu_uart1_recv_stop(               void                )
{
        return( false );
}


uint32_t bsp_mcu_uart1_dma_recv_ndtr_get(       void            )
{
        return( huart1.hdmarx->Instance->NDTR );
}


void bsp_mcu_uart1_dma_tx_isr(                  void            )
{
        HAL_DMA_IRQHandler( huart1.hdmatx );
}


void bsp_mcu_uart1_dma_rx_isr(                  void )
{
        HAL_DMA_IRQHandler( huart1.hdmarx );
}


bool bsp_mcu_uart1_xmit_start(                  uint8_t *               data,
                                                size_t                  size )
{
        HAL_StatusTypeDef       resp;


        //while( HAL_UART_GetState( &huart1 ) != HAL_UART_STATE_READY );
        resp    =   HAL_UART_Transmit_DMA( &huart1, data, size );

        return( resp == HAL_OK ? false : true );
}


uint32_t bsp_mcu_uart1_recv_dma_head_get(       void )
{
        return( huart1.hdmarx->Instance->NDTR );
        //return( DMA2_Stream2->NDTR );
}


bool    bsp_mcu_uart1_sts_idle(                 void )
{
        bool    sts_idle        =   READ_BIT( huart1.Instance->SR, USART_SR_IDLE );

        return( sts_idle );
}
