/**
  * @file    gnss_time_sync.c
  * @brief   GNSS Time Synchronization
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<stdio.h>
#include	"gnss.h"

/**
 * @brief NMEA Time Synchronization
 */
void gnss_time_sync(                        gnss_t *            gnss,
	                                    time_t *            time_dat )
{
	struct  tm      orig;


	orig.tm_sec     =   gnss->nmea.rmc.tm_dat.tm_sec;
	orig.tm_min     =   gnss->nmea.rmc.tm_dat.tm_min;
	orig.tm_hour    =   gnss->nmea.rmc.tm_dat.tm_hour;
	orig.tm_mday    =   gnss->nmea.rmc.tm_dat.tm_mday;
	orig.tm_mon     =   gnss->nmea.rmc.tm_dat.tm_mon - 1;
	orig.tm_year    =   gnss->nmea.rmc.tm_dat.tm_year + 100;
	//orig.tm_wday    =   5;
	//orig.tm_yday    =   19;
	orig.tm_isdst   =   -1;

	*time_dat       =   mktime( &orig );
}
