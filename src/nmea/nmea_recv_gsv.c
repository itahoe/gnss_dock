/**
  * @file    nmea_recv_gsv.c
  * @brief   NMEA GSV sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea.h"

/**
 * @brief Satellites in view
 */
nmea_pckt_type_t        nmea_recv_gsv(          nmea_gsv_t *        gsv,
                                        const   char *              str )
{
	nmea_pckt_type_t        pckt_type       =   NMEA_PCKT_TYPE_GSV;
	int             msg_num;


	nmea_str2int( &gsv->msgs_total, str );

	while( *str++ != ',' );

	nmea_str2int( &msg_num, str );

	while( *str++ != ',' );

	nmea_str2int( &gsv->sats_in_view, str );

	for( size_t i = 0; i < msg_num; i++ )
	{
	}

	return( pckt_type );
}
