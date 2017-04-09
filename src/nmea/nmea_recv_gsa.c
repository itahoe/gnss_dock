/**
  * @file    nmea_recv_gsa.c
  * @brief   NMEA GSA sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea.h"

/**
 * @brief GPS DOP and active satellites
 */
nmea_pckt_type_t        nmea_recv_gsa(          nmea_gsa_t *        gsa,
                                        const   char *              str )
{
	nmea_pckt_type_t        pckt_type       =   NMEA_PCKT_TYPE_GSA;
	//bool            resp            =   true;
	size_t          sat_id_sizeof   =   sizeof( gsa->sat_id ) / sizeof( int );


	switch( *str++ )
	{
		case 'A':   gsa->sel_mode       =   NMEA_SEL_MODE_AUTO;     break;
		case 'M':   gsa->sel_mode       =   NMEA_SEL_MODE_MANUAL;   break;
		default:    gsa->sel_mode       =   NMEA_SEL_MODE_ERROR;    break;
	}

	while( *str++ != ',' );

	switch( *str++ )
	{
		case '3':   gsa->fix_mode       =   NMEA_FIX_MODE_3D;       break;
		case '2':   gsa->fix_mode       =   NMEA_FIX_MODE_2D;       break;
		case '1':   gsa->fix_mode       =   NMEA_FIX_MODE_NONE;     break;
		default:    gsa->fix_mode       =   NMEA_FIX_MODE_ERROR;    break;
	}

	for( size_t i = 0; i < sat_id_sizeof; i++ )
	{
		while( *str++ != ',' );
		nmea_str2int( &gsa->sat_id[i], str );
	}

	while( *str++ != ',' );

	nmea_str2flt( &gsa->pdop, str );

	while( *str++ != ',' );

	nmea_str2flt( &gsa->hdop, str );

	while( *str++ != ',' );

	nmea_str2flt( &gsa->vdop, str );

	return( pckt_type );
}
