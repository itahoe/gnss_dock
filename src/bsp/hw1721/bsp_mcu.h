/**
  * @file    bsp_mcu.h
  * @brief   BSP - MCU services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef  BSP_MCU_H
#define  BSP_MCU_H


#include <stdbool.h>
#include <stdio.h>
#include "bsp.h"


bool bsp_mcu_uart1_init(                const   size_t                  baud );

bool bsp_mcu_uart2_init(                const   size_t                  baud );

bool bsp_mcu_uart3_init(                const   size_t                  baud );

bool bsp_mcu_uart6_init(                const   size_t                  baud );


bool bsp_mcu_uart1_recv_start(                  uint8_t *               data,
                                                size_t                  size );

bool bsp_mcu_uart2_recv_start(                  uint8_t *               data,
                                                size_t                  size );

bool bsp_mcu_uart3_recv_start(                  uint8_t *               data,
                                                size_t                  size );

bool bsp_mcu_uart6_recv_start(                  uint8_t *               data,
                                                size_t                  size );


bool bsp_mcu_uart1_recv_stop(                   void                        );

bool bsp_mcu_uart2_recv_stop(                   void                        );

bool bsp_mcu_uart3_recv_stop(                   void                        );

bool bsp_mcu_uart6_recv_stop(                   void                        );


void bsp_mcu_uart1_isr(                         void );

void bsp_mcu_uart2_isr(                         void );

void bsp_mcu_uart3_isr(                         void );

void bsp_mcu_uart6_isr(                         void );


uint32_t bsp_mcu_uart1_dma_recv_ndtr_get(       void );

uint32_t bsp_mcu_uart2_dma_recv_ndtr_get(       void );

uint32_t bsp_mcu_uart3_dma_recv_ndtr_get(       void );

uint32_t bsp_mcu_uart6_dma_recv_ndtr_get(       void );


void bsp_mcu_uart1_dma_tx_isr(                  void );

void bsp_mcu_uart2_dma_tx_isr(                  void );

void bsp_mcu_uart3_dma_tx_isr(                  void );

void bsp_mcu_uart6_dma_tx_isr(                  void );


void bsp_mcu_uart1_dma_rx_isr(                  void );

void bsp_mcu_uart2_dma_rx_isr(                  void );

void bsp_mcu_uart3_dma_rx_isr(                  void );

void bsp_mcu_uart6_dma_rx_isr(                  void );


bool bsp_mcu_uart1_xmit_start(                  uint8_t *               data,
                                                size_t                  size );

bool bsp_mcu_uart2_xmit_start(                  uint8_t *               data,
                                                size_t                  size );

bool bsp_mcu_uart3_xmit_start(                  uint8_t *               data,
                                                size_t                  size );

bool bsp_mcu_uart6_xmit_start(                  uint8_t *               data,
                                                size_t                  size );


uint32_t bsp_mcu_uart6_recv_size_get(           void );


uint32_t bsp_mcu_uart1_recv_dma_head_get(       void );

uint32_t bsp_mcu_uart2_recv_dma_head_get(       void );

uint32_t bsp_mcu_uart3_recv_dma_head_get(       void );
//uint32_t bsp_mcu_uart3_read_ndtr(               void                    );

uint32_t bsp_mcu_uart6_recv_dma_head_get(       void );


bool bsp_mcu_uart1_recv_dma_full_get(           void );

bool    bsp_mcu_uart1_sts_idle(                 void );

bool    bsp_mcu_uart2_sts_idle(                 void );

bool    bsp_mcu_uart3_sts_idle(                 void );

bool    bsp_mcu_uart6_sts_idle(                 void );


#endif	//BSP_MCU_H
