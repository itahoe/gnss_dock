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

void	bsp_gnss_xmit( const char * p, size_t len );

#endif	//BSP_GNSS_H
