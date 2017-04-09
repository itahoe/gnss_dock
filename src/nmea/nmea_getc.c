/**
  * @file    nmea_getc.c
  * @brief   NMEA service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<stdio.h>
#include	"nmea.h"

/**
 * @brief NMEA recieve char
 */
nmea_pckt_type_t    nmea_getc(              nmea_t *            p,
                                            char                c )
{
/*
        NMEA_PCKT_TYPE_NONE,
        NMEA_PCKT_TYPE_GGA,
        NMEA_PCKT_TYPE_GSA,
        NMEA_PCKT_TYPE_GSV,
        NMEA_PCKT_TYPE_RMC,
        NMEA_PCKT_TYPE_X,
*/
        nmea_pckt_type_t    type        =   NMEA_PCKT_TYPE_NONE;

        switch( p->state )
        {
                case NMEA_STATE_IDLE:
                        if( c == '$' )
                        {
                                p->chksum       =   0;
                                p->state        =   NMEA_STATE_RECV;
                        }
			break;

                case NMEA_STATE_RECV:
                        if( c == '*' )
                        {
                                p->state        =   NMEA_STATE_CHKSUM;
                        }
                        else
                        {
                                p->chksum       ^=  c;
                        }
			break;

                case NMEA_STATE_CHKSUM:
			break;

                default:
                        p->state        =   NMEA_STATE_IDLE;
			break;
        }


        return( type );
}
