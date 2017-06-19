/**
  * @file    nvg_log_recv.c
  * @brief   NVG LOG sentence recieve
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <stdio.h>
#include "app.h"
#include "nvg.h"
#include "nmea.h"


static
nvg_type_t nvg_recv_bss(                        nvg_bss_t *             p,
                                        const   char *                  str )
{
	nvg_type_t      type    =   NVG_TYPE_INVALID;

        return( type );
}


static
nvg_type_t nvg_recv_ver(                        nvg_ver_t *             p,
                                        const   char *                  str )
{
	nvg_type_t      type    =   NVG_TYPE_INVALID;

        return( type );
}


static
nvg_type_t nvg_recv_log(                        nvg_log_t *             p,
                                        const   char *                  str )
{
	nvg_type_t      type    =   NVG_TYPE_INVALID;


        if( strncmp( str, "MODE,", sizeof("MODE")-1 ) == 0 )
        {
                str             +=  sizeof("MODE")-1;
                //type            =   NVG_TYPE_LOG_MODE;

                if( *str == ',' )
                {
                        type            =   NVG_TYPE_LOG_MODE_SET;
                        str++;
                        p->mode         =   (nvg_log_mode_t) strtol( str, NULL, 10 );
                        app_cfg_write( RTC_BKP_DR0, p->mode );
                }
                else if( *str == '*' )
                {
                        type            =   NVG_TYPE_LOG_MODE_GET;
                        str++;
                }
        }
        else if( strncmp( str, "CLOSE,", sizeof("CLOSE")-1 ) == 0 )
        {
                str             +=  sizeof("CLOSE")-1;

                if( *str == '*' )
                {
                        str++;
                        type            =   NVG_TYPE_LOG_CLOSE;
                }
        }
        else if( strncmp( str, "OPEN,", sizeof("OPEN")-1 ) == 0 )
        {
                str             +=  sizeof("OPEN")-1;

                if( *str == '*' )
                {
                        str++;
                        type            =   NVG_TYPE_LOG_OPEN;
                }
        }
        else if( strncmp( str, "RESTART,", sizeof("RESTART")-1 ) == 0 )
        {
                str             +=  sizeof("RESTART")-1;

                if( *str == '*' )
                {
                        str++;
                        type            =   NVG_TYPE_LOG_RESTART;
                }
        }

        return( type );
}


/**
 * @brief GNSS recieve char hook
 */
nvg_type_t nvg_recv(                            nvg_t *                 p,
                                        const   char *                  str )
{
	nvg_type_t      type    =   NVG_TYPE_INVALID;
        size_t          len;
	uint8_t         chksum;
	int             est;


	len             =   nmea_chksum( &chksum, str, '*' );
	sscanf( str + len, "*%02X", &est );

	if(  chksum != (uint8_t) est )
	{
		return( type );
	}

	if(         str[0] == 'G' ) //sentence type: general
	{
	}
	else if(    str[0] == 'P' ) //sentence type: proprietary
	{
		if(         (str[1] == 'N') && (str[2] == 'V') && (str[3] == 'G') )
		{
                        if(         strncmp( str+4, "BSS,", sizeof("BSS")-1 ) == 0 )
                        {
                                type    =   nvg_recv_bss( &p->bss, str+8 );
                        }
                        else if(    strncmp( str+4, "VER,", sizeof("VER")-1 ) == 0 )
                        {
                                type    =   nvg_recv_ver( &p->ver, str+8 );
                        }
                        else if(    strncmp( str+4, "LOG,", sizeof("LOG")-1 ) == 0 )
                        {
                                type    =   nvg_recv_log( &p->log, str+8 );
                        }
                        else
                        {
                                //type    =   NVG_TYPE_INVALID;
                        }
		}
	}

	return( type );
}
