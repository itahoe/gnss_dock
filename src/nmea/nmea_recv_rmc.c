/**
  * @file    nmea_recv_rmc.c
  * @brief   NMEA RMC sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"nmea.h"

/**
 * @brief Recommended Minimum Navigation Information
 */
bool nmea_recv_rmc(                         nmea_rmc_t *        rmc,
                                    const   char *              str )
{
	bool            resp            =   true;


	nmea_str2time( &rmc->tm_dat, str );

	while( *str++ != ',' );

	switch( *str++ )
	{
		case 'A':	rmc->status     =   NMEA_STS_VALID;	break;
		case 'V':	rmc->status     =   NMEA_STS_INVALID;	break;
		default:	rmc->status     =   NMEA_STS_ERROR;	break;
	}

	while( *str++ != ',' );

	nmea_str2flt( &rmc->latitude, str );

	while( *str++ != ',' );

	if( *str++ == 'S')
	{
		rmc->latitude   =   -( rmc->latitude );
	}

	while( *str++ != ',' );

	nmea_str2flt( &rmc->longitude, str );

	while( *str++ != ',' );

	if( *str++ == 'W')
	{
		rmc->longitude  =   -( rmc->longitude );
	}

	while( *str++ != ',' );

	nmea_str2flt( &rmc->speed, str );

	while( *str++ != ',' );

	nmea_str2flt( &rmc->track_angle, str );

	while( *str++ != ',' );

	nmea_str2date( &rmc->tm_dat, str );

	while( *str++ != ',' );

	nmea_str2flt( &rmc->magnetic_variation, str );

	return( resp );
}
