/**
  * @file    bsp_key.h
  * @brief   Board Support Package - Key Services
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  BSP_KEY_H
#define  BSP_KEY_H

#include	<stdbool.h>

void	bsp_key0_init( void );
void	bsp_key1_init( void );

bool	bsp_key0_get( void );
bool	bsp_key1_get( void );

#endif	//BSP_KEY_H
