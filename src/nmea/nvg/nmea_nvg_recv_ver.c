/**
  * @file    nmea_nvg_recv_ver.c
  * @brief   NVG VER sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <stdlib.h>
#include "nmea_nvg.h"

/**
 * @brief NVG proptietary sentence, version info
 */
bool nmea_nvg_recv_ver(                     nmea_nvg_ver_t  *   p,
                                    const   char *              str )
{
	bool            resp            =   true;


	while( *str++ != ',' );

	while( *str++ != ',' );

        p->num          =   strtol( str, NULL, 10 );

	return( resp );
}
