/**
  * @file    nmea_nv_recvg_log.c
  * @brief   NVG LOG sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea_nvg.h"

/**
 * @brief NVG proptietary sentence, log control
 */
bool nmea_nvg_recv_log(                     nmea_nvg_log_t  *   p,
                                    const   char *              str )
{
	bool            resp;

	while( *str++ != ',' );

        if(         strncmp( str, "MODE,", sizeof("MODE") ) == 0 )
        {
                resp    =   true;
        }
        else
        {
                resp    =   false;
        }

	return( resp );
}
