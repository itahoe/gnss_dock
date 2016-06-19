/**
  * @file    nmea.c
  * @brief   NMEA service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<stdio.h>
#include	"nmea.h"

/**
 * @brief NMEA initialization
 */
void nmea_init(                             nmea_t *            nmea )
{
	memset( nmea, 0, sizeof( nmea_t ) );
}
