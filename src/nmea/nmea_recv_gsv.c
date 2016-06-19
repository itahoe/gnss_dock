/**
  * @file    nmea_recv_gsv.c
  * @brief   NMEA GSV sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea.h"

/**
 * @brief Satellites in view
 */
bool nmea_recv_gsv(                         nmea_gsv_t *        gsv,
                                    const   char *              str )
{
	bool            resp            =   true;
	int             msg_num;


	while( *str++ != ',' );

	nmea_str2int( &gsv->msgs_total, str );

	while( *str++ != ',' );

	nmea_str2int( &msg_num, str );

	while( *str++ != ',' );

	nmea_str2int( &gsv->sats_in_view, str );

	for( size_t i = 0; i < msg_num; i++ )
	{
	}

	return( resp );
}
