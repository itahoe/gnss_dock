/**
  * @file    flog.h
  * @brief   File Manager
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	<time.h>
#include	"bsp_flog.h"
#include	"flog.h"
#include	"diskio.h"
#include	"ff.h"
#include	"app.h"

FATFS   SDFatFs;  /* File system object for SD card logical drive */
char    SDPath[4]; /* SD card logical drive path */

/*
static
bool	flog_file_expired(              flog_t *                p )
{
	bool        resp        =   false;

	return( resp );
}
*/

/**
 * @brief File Manager initialization
 */
void	flog_init(                      flog_t *                p )
{
	bsp_flog_init();

	if( f_mount(&SDFatFs, (TCHAR const*)SDPath, 1 ) != FR_OK )
	{
		//app_error();	//FatFs Initialization Error
	}
	else
	{
		p->sts.ready    =   true;   
	}
}

/**
 * @brief File Manager file close
 */
void	flog_close(                     flog_t *                p )
{
	FRESULT         resp;

	resp    =   f_close( &p->file_log );
	APP_TRACE( "f_close() - " );
	APP_TRACE_FF_RES( resp );
	resp    =   resp;
}

/**
 * @brief File Manager file open
 */
void	flog_open(                      flog_t *                p )
{
	FRESULT         resp;


	flog_name_compose( p->fname, "log", sizeof( p->fname ) );

	resp    =   f_open( &p->file_log, p->fname, FA_CREATE_ALWAYS | FA_WRITE);

	APP_TRACE( "f_open() - " );
	APP_TRACE_FF_RES( resp );

	if( resp != FR_OK )
	{
		//resp    =   f_close( &p->file_log );
		//APP_TRACE( "f_close() - " );
		//ff_trace_res( resp );

		resp    =   f_mount( NULL, (TCHAR const*) SDPath, 1 );
		APP_TRACE( "f_mount(NULL) - " );
		APP_TRACE_FF_RES( resp );

		//FATFS_UnLinkDriver( SDPath );

		//FATFS_LinkDriver( &SD_Driver, SDPath );

		resp    =   f_mount( &SDFatFs, (TCHAR const*) SDPath, 1 );
		APP_TRACE( "f_mount(SD) - " );
		APP_TRACE_FF_RES( resp );

		if( resp == FR_OK )
		{
			resp    =   f_open( &p->file_log, p->fname, FA_CREATE_ALWAYS | FA_WRITE);

			APP_TRACE( "f_open() - " );
			APP_TRACE_FF_RES( resp );
		}
	}
}

/**
 * @brief File Manager write
 */
void	flog_write(                     flog_t *                p,
	                        const   uint8_t *               pbuf,
	                                size_t                  len )
{
	FRESULT         res;
	uint32_t        wr_bytes        =   0;


	while( len > 0 )
	{
		res     =   f_write( &p->file_log, pbuf, len, (void *) &wr_bytes );

		if( res == FR_OK )
		{
			if( wr_bytes < len )
			{
				APP_TRACE( "f_write() - len: %d, wr_bytes: %d\n", len, wr_bytes );
			}
			else
			{
				f_sync( &p->file_log );
			}

			len     -=  wr_bytes;
			pbuf    +=  wr_bytes;
		}
		else
		{
			APP_TRACE( "f_write() - " );
			APP_TRACE_FF_RES( res );
			APP_TRACE( "pbuf: %0X\n", pbuf );
			APP_TRACE( "len: %d\n", len );
			APP_TRACE( "wr_bytes: %d\n", wr_bytes );

			break;
		}
	}
}
