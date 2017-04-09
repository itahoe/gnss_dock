/**
  * @file    nmea_nvg_log_recv.c
  * @brief   NVG LOG sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea_nvg.h"

/**
 * @brief NVG proptietary sentence, log control
 */
bool nmea_nvg_recv(                         nmea_nvg_t *        p,
                                    const   char *              str )
{
	bool            resp;


        if(         strncmp( str, "LOG,", sizeof("LOG")-1 ) == 0 )
        {
                resp    =   nmea_nvg_recv_log( &p->log, str + sizeof("LOG") );
        }
        else if(    strncmp( str, "VER,", sizeof("VER")-1 ) == 0 )
        {
                resp    =   nmea_nvg_recv_ver( &p->ver, str + sizeof("VER") );
        }
        else if(    strncmp( str, "BSS,", sizeof("BSS")-1 ) == 0 )
        {
                resp    =   nmea_nvg_recv_bss( &p->bss, str + sizeof("BSS") );
        }
        else
        {
                resp    =   false;
        }

	return( resp );
}
