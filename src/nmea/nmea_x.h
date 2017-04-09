/**
  * @file    nmea_x.h
  * @brief   NMEA service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NMEA_X_H
#define  NMEA_X_H


#include "nmea_nvg.h"


typedef	struct	nmea_x_s
{
        nmea_nvg_t      nvg;
} nmea_x_t;


bool nmea_x_recv( nmea_x_t * x, const char * str );


#endif	//NMEA_X_H
