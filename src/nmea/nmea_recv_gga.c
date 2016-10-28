/**
  * @file    nmea_recv_gga.c
  * @brief   NMEA GGA sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea.h"

/**
 * @brief Global Positioning System Fix Data. Time, Position and fix related data for a GPS receiver
 */
bool nmea_recv_gga(                         nmea_gga_t *        gga,
                                    const   char *              str )
{
	bool            resp            =   true;


	nmea_str2time( &gga->tm_dat, str );

	while( *str++ != ',' );

	nmea_str2flt( &gga->latitude, str );

	while( *str++ != ',' );

	if( *str++ == 'S')
	{
		gga->latitude   =   -( gga->latitude );
	}

	while( *str++ != ',' );

	nmea_str2flt( &gga->longitude, str );

	while( *str++ != ',' );

	if( *str++ == 'W')
	{
		gga->longitude   =   -( gga->longitude );
	}

	while( *str++ != ',' );

	switch( *str )
	{
		case '0':   gga->fix    =   NMEA_FIX_NONE;          break;
		case '1':   gga->fix    =   NMEA_FIX_SPS;           break;
		case '2':   gga->fix    =   NMEA_FIX_PPS;           break;
		case '4':   gga->fix    =   NMEA_FIX_RTK_INT;       break;
		case '5':   gga->fix    =   NMEA_FIX_RTK_FLT;       break;
		default:    gga->fix    =   NMEA_FIX_ERROR;         break;
	}

	while( *str++ != ',' );

	nmea_str2int( &gga->sats_used, str );

	while( *str++ != ',' );

	nmea_str2flt( &gga->hdop, str );

	while( *str++ != ',' );

	nmea_str2flt( &gga->altitude, str );

	while( *str++ != ',' );

	//skip next field, it's always 'M'

	while( *str++ != ',' );

	nmea_str2flt( &gga->geoidal_separation, str );

	while( *str++ != ',' );

	//skip next field, it's always 'M'

	while( *str++ != ',' );

	nmea_str2flt( &gga->dgps_data_age_sec, str );

	while( *str++ != ',' );

	nmea_str2int( &gga->dgps_station_id, str );

	return( resp );
}
