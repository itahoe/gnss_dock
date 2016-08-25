/**
  * @file    bsp_gnss.h
  * @brief   Board Support Package - GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_GNSS_H
#define  BSP_GNSS_H

#include <stdbool.h>
#include <stdio.h>
#include "bsp.h"

void	bsp_gnss_init( void );

void bsp_gnss_xmit(                 const   char *              data,
                                            size_t              size );

void bsp_gnss_recv_start(                   uint8_t *           data,
                                            size_t              size );

void bsp_gnss_recv_stop( void );

void bsp_gnss_xmit_start(                   uint8_t *           data,
                                            size_t              size );

void bsp_gnss_xmit_stop( void );

uint32_t bsp_gnss_recv_fifo_head_get( void );

uint32_t bsp_gnss_recv_fifo_size_get( void );

#endif	//BSP_GNSS_H
