/**
  * @file    bsp_mcu_uart3.c
  * @brief   BSP MCU UART3
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_mcu.h"
#include "bsp.h"


static  DMA_HandleTypeDef       huart3_dma_tx;
static  DMA_HandleTypeDef       huart3_dma_rx;
static  UART_HandleTypeDef      huart3;


static
void bsp_mcu_uart3_io_init( void )
{
        GPIO_InitTypeDef    gpio_tx     =   {    .Pin       =    GPIO_PIN_10,
	                                             .Mode      =    GPIO_MODE_AF_PP,
	                                             .Pull      =    GPIO_PULLUP,
	                                             .Speed     =    GPIO_SPEED_FREQ_LOW,
	                                             .Alternate =    GPIO_AF7_USART3 };

        GPIO_InitTypeDef    gpio_rx     =   {    .Pin       =    GPIO_PIN_5,
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
int bsp_mcu_uart3_dma_init( void )
{
        int                     resp    =   0;


        __HAL_RCC_DMA1_CLK_ENABLE();

        huart3_dma_tx.Instance                  =   DMA1_Stream3;
        huart3_dma_tx.Init.Channel              =   DMA_CHANNEL_4;
        huart3_dma_tx.Init.Direction            =   DMA_MEMORY_TO_PERIPH;
        huart3_dma_tx.Init.PeriphInc            =   DMA_PINC_DISABLE;
        huart3_dma_tx.Init.MemInc               =   DMA_MINC_ENABLE;
        huart3_dma_tx.Init.PeriphDataAlignment  =   DMA_PDATAALIGN_BYTE;
        huart3_dma_tx.Init.MemDataAlignment     =   DMA_MDATAALIGN_BYTE;
        huart3_dma_tx.Init.Mode                 =   DMA_NORMAL;
        huart3_dma_tx.Init.Priority             =   DMA_PRIORITY_LOW;
        huart3_dma_tx.Init.FIFOMode             =   DMA_FIFOMODE_DISABLE;
        huart3_dma_tx.Init.FIFOThreshold        =   DMA_FIFO_THRESHOLD_FULL;
        huart3_dma_tx.Init.MemBurst             =   DMA_MBURST_INC4;
        huart3_dma_tx.Init.PeriphBurst          =   DMA_PBURST_INC4;

        HAL_DMA_Init( &huart3_dma_tx );
        __HAL_LINKDMA( &huart3, hdmatx, huart3_dma_tx );

        huart3_dma_rx.Instance                  =   DMA1_Stream1;
        huart3_dma_rx.Init.Channel              =   DMA_CHANNEL_4;
        huart3_dma_rx.Init.Direction            =   DMA_PERIPH_TO_MEMORY;
        huart3_dma_rx.Init.PeriphInc            =   DMA_PINC_DISABLE;
        huart3_dma_rx.Init.MemInc               =   DMA_MINC_ENABLE;
        huart3_dma_rx.Init.PeriphDataAlignment  =   DMA_PDATAALIGN_BYTE;
        huart3_dma_rx.Init.MemDataAlignment     =   DMA_MDATAALIGN_BYTE;
        huart3_dma_rx.Init.Mode                 =   DMA_CIRCULAR;
        huart3_dma_rx.Init.Priority             =   DMA_PRIORITY_HIGH;
        huart3_dma_rx.Init.FIFOMode             =   DMA_FIFOMODE_DISABLE;
        huart3_dma_rx.Init.FIFOThreshold        =   DMA_FIFO_THRESHOLD_FULL;
        huart3_dma_rx.Init.MemBurst             =   DMA_MBURST_INC4;
        huart3_dma_rx.Init.PeriphBurst          =   DMA_PBURST_INC4;

        HAL_DMA_Init( &huart3_dma_rx );
        __HAL_LINKDMA( &huart3, hdmarx, huart3_dma_rx );

        return( resp );
}


bool bsp_mcu_uart3_init(                const   size_t                  baud )
{
        bool        resp    =   false;

        huart3.Instance                         =   USART3;
        huart3.Init.BaudRate                    =   baud;
        huart3.Init.WordLength                  =   UART_WORDLENGTH_8B;
        huart3.Init.StopBits                    =   UART_STOPBITS_1;
        huart3.Init.Parity                      =   UART_PARITY_NONE;
        huart3.Init.Mode                        =   UART_MODE_TX_RX;
        huart3.Init.HwFlowCtl                   =   UART_HWCONTROL_NONE;
        huart3.Init.OverSampling                =   UART_OVERSAMPLING_16;

        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();

        HAL_UART_Init( &huart3 );

        bsp_mcu_uart3_io_init();
        bsp_mcu_uart3_dma_init();

        HAL_NVIC_SetPriority(   DMA1_Stream3_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_TX, 0 );
        HAL_NVIC_EnableIRQ(     DMA1_Stream3_IRQn );

        HAL_NVIC_SetPriority(   DMA1_Stream1_IRQn,      BSP_NVIC_PRIO_GNSS_DMA_RX, 0 );
        HAL_NVIC_EnableIRQ(     DMA1_Stream1_IRQn );

        HAL_NVIC_SetPriority(   USART3_IRQn,            BSP_NVIC_PRIO_GNSS_RECV_SMBL, 0 );
        HAL_NVIC_EnableIRQ( USART3_IRQn );

        return( resp );
}


void bsp_mcu_uart3_isr(                         void )
{
        HAL_UART_IRQHandler( &huart3 );
}


bool bsp_mcu_uart3_recv_start(              uint8_t *           data,
                                            size_t              size )
{
        bool        resp    =   false;

        HAL_UART_Receive_DMA( &huart3, data, size );

        //SET_BIT( huart3.Instance->CR1,  USART_CR1_IDLEIE        );

        return( resp );
}


bool bsp_mcu_uart3_recv_stop(               void                )
{
        return( false );
}


uint32_t bsp_mcu_uart3_dma_recv_ndtr_get(       void            )
{
        return( huart3.hdmarx->Instance->NDTR );
}


void bsp_mcu_uart3_dma_tx_isr(                  void            )
{
        HAL_DMA_IRQHandler( huart3.hdmatx );
}


void bsp_mcu_uart3_dma_rx_isr(                  void            )
{
        HAL_DMA_IRQHandler( huart3.hdmarx );
}


bool bsp_mcu_uart3_xmit_start(                  uint8_t *               data,
                                                size_t                  size )
{
        HAL_StatusTypeDef       resp;


        //while( HAL_UART_GetState( &huart3 ) != HAL_UART_STATE_READY );
        resp    =   HAL_UART_Transmit_DMA( &huart3, data, size );

        return( resp == HAL_OK ? false : true );
}

bool    bsp_mcu_uart3_sts_idle(                 void )
{
        bool    sts_idle        =   READ_BIT( huart3.Instance->SR, USART_SR_IDLE );

        return( sts_idle );
}
