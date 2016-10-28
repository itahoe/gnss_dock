/**
  * @file    nmea_x.h
  * @brief   NMEA service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NMEA_X_H
#define  NMEA_X_H


#include	<stdbool.h>
#include	<string.h>
#include	"config.h"


typedef	struct	nmea_ext_s
{
        size_t                  reserved;
} nmea_ext_t;


bool nmea_recv_x( nmea_ext_t * ext, const char * str );

#endif	//NMEA_X_H
