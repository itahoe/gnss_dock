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
bool    nvg_recv_bss(                           nvg_bss_t *             p,
                                        const   char *                  str )
{
        bool            resp            =   false;


        APP_TRACE( "NVGBSS: %s\n", str );

        return( resp );
}


static
bool    nvg_recv_log(                           nvg_log_t *             p,
                                        const   char *                  str )
{
        bool            resp            =   false;


        if( strncmp( str, "MODE,", sizeof("MODE")-1 ) == 0 )
        {
                str             +=  sizeof("MODE")-1;

                if( *str == ',' )
                {
                        str++;
                        p->mode         =   (nvg_log_mode_t) strtol( str, NULL, 10 );
                        app_cfg_write( RTC_BKP_DR0, p->mode );

                        resp            =   true;
                }
                else if( *str == '*' )
                {
                        str++;
                        resp            =   true;
                }

        }

        return( resp );
}


static
bool    nvg_recv_ver(                           nvg_ver_t *             p,
                                        const   char *                  str )
{
        bool            resp            =   false;


        APP_TRACE( "NVGVER: %s\n", str );

        return( resp );
}


/**
 * @brief GNSS recieve char hook
 */
bool nvg_recv(                                  nvg_t *                 p,
                                        const   char *                  str )
{
	bool            resp    =   false;
        size_t          len;
	uint8_t         chksum;
	int             est;


	len             =   nmea_chksum( &chksum, str, '*' );
	sscanf( str + len, "*%02X", &est );

	if(  chksum != (uint8_t) est )
	{
		return( resp );
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
                                p->type =   NVG_TYPE_BSS;
                                resp    =   nvg_recv_bss( &p->bss, str+8 );
                        }
                        else if(    strncmp( str+4, "LOG,", sizeof("LOG")-1 ) == 0 )
                        {
                                p->type =  NVG_TYPE_LOG;
                                resp    =   nvg_recv_log( &p->log, str+8 );
                        }
                        else if(    strncmp( str+4, "VER,", sizeof("VER")-1 ) == 0 )
                        {
                                p->type =  NVG_TYPE_VER;
                                resp    =   nvg_recv_ver( &p->ver, str+8 );
                        }
                        else
                        {
                                p->type =  NVG_TYPE_INVALID;
                                resp    =   true;
                        }
		}
	}

	return( resp );
}
