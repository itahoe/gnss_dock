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

		if(         strncmp( str+2, "GGA,", sizeof("GGA") ) == 0 )
		{
			resp    =   nmea_recv_gga( &nmea->gga, str+6 );
		}
		else if(    strncmp( str+2, "GSA,", sizeof("GSA") ) == 0 )
		{
			resp    =   nmea_recv_gsa( &nmea->gsa, str+6 );
		}
		else if(    strncmp( str+2, "GSV,", sizeof("GSV") ) == 0 )
		{
			resp    =   nmea_recv_gsv( &nmea->gsv, str+6 );
		}
		else if(    strncmp( str+2, "RMC,", sizeof("RMC") ) == 0 )
		{
			resp    =   nmea_recv_rmc( &nmea->rmc, str+6 );
		}
		else
		{
			resp    =   true;
		}
	}
	else if(    str[0] == 'P' ) //sentence type: proprietary
	{
                resp    =   nmea_x_recv( &nmea->x, str+6 );
	}
	else
	{
		resp    =   false; //sentence type: unsupported
	}

	return( resp );
}
