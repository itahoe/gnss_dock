/**
  * @file    nmea_recv.c
  * @brief   NMEA recieve service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<stdio.h>
#include	"nmea.h"

/**
 * @brief NMEA sentence recieve
 */
bool nmea_recv(                             nmea_t *            nmea,
                                    const   char *              str )
{
	bool            resp    =   false;
	uint8_t         chksum;
	int             est;
	size_t          len;


	nmea->recv_total++;

	len             =   nmea_chksum( &chksum, str, '*' );
	sscanf( str + len, "*%02X", &est );

	if(  chksum != (uint8_t) est )
	{
		nmea->chksum_errors++;

		return( resp );
	}

	if(         str[0] == 'G' ) //sentence type: general
	{
		switch( str[1] )
		{
			case 'A':       nmea->nsid  =   NMEA_NSID_GALILEO;      break;
			case 'L':       nmea->nsid  =   NMEA_NSID_GLONASS;      break;
			case 'P':       nmea->nsid  =   NMEA_NSID_GPS;          break;
			case 'N':       nmea->nsid  =   NMEA_NSID_MULTI;        break;
			default:        nmea->nsid  =   NMEA_NSID_INVALID;      break;
		}

		if(         strncmp( &str[2], "GGA", 3) == 0 )
		{
			resp    =   nmea_recv_gga( &nmea->gga, &str[5] );
		}
		else if(    strncmp( &str[2], "GSA", 3) == 0 )
		{
			resp    =   nmea_recv_gsa( &nmea->gsa, &str[5] );
		}
		else if(    strncmp( &str[2], "GSV", 3) == 0 )
		{
			resp    =   nmea_recv_gsv( &nmea->gsv, &str[5] );
		}
		else if(    strncmp( &str[2], "RMC", 3) == 0 )
		{
			resp    =   nmea_recv_rmc( &nmea->rmc, &str[5] );
		}
		else
		{
			resp    =   true;
		}
	}
	else if(    str[0] == 'P' ) //sentence type: proprietary
	{
		resp    =   true;
	}
	else                        //sentence type: unsupported
	{
	}

	return( resp );
}
