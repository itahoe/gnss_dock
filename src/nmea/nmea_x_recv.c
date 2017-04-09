/**
  * @file    nmea_x_recv.c
  * @brief   NMEA proprietary sentences recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "nmea_x.h"
#include "nmea_nvg.h"


/**
 * @brief External sentence interface
 */
bool nmea_x_recv(                               nmea_x_t *      x,
                                        const   char *          str )
{
        bool            resp;

        if(         strncmp( str, "NVG,", sizeof("NVG")-1 ) == 0 )
        {
                //resp    =   nmea_nvg_recv( &x->nvg, str + sizeof("NVG")-1 );
        }
        else
        {
                resp    =   false;
        }

        return( resp );
}
