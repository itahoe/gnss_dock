/**
  * @file    flog.h
  * @brief   File Manager
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <time.h>
//#include "bsp_storage.h"
#include "storage.h"
#include "diskio.h"
#include "ff.h"
#include "app.h"


static  FATFS           SDFatFs;
static  char            SDPath[4];


/**
 * @brief File Manager initialization
 */
void storage_init( void )
{
	//bsp_storage_init();
}

/**
 * @brief File Manager file close
 */
void storage_close(                             storage_t *             p )
{
	p->fresult      =   f_close( &p->file_log );

        if( p->fresult == FR_OK )
        {
                p->sts.enable   =   false;
        }
        else
        {
                p->sts.enable   =   false;
        }
}

/**
 * @brief File Manager file open
 */
bool storage_open(                              storage_t *             p )
{
        bool            resp    =   false;

	storage_name_compose( p->fname, p->fext, sizeof( p->fname ) );

	p->fresult      =   f_open( &p->file_log, p->fname, FA_CREATE_ALWAYS | FA_WRITE);

	if( p->fresult == FR_OK )
        {
                p->sts.enable   =   true;
                resp            =   true;
        }
        else
	{
		p->fresult      =   f_mount( NULL, (TCHAR const*) SDPath, 1 );
		p->fresult      =   f_mount( &SDFatFs, (TCHAR const*) SDPath, 1 );

		if( p->fresult == FR_OK )
		{
			p->fresult      =   f_open( &p->file_log, p->fname, FA_CREATE_ALWAYS | FA_WRITE);
                        p->sts.enable   =   (p->fresult == FR_OK) ? true : false;
                        resp            =   (p->fresult == FR_OK) ? true : false;
		}
                else
                {
                        p->sts.enable   =   false;
                }
	}

        return( resp );
}

/**
 * @brief File Manager write
 */
void storage_write(                             storage_t *             p,
                                        const   uint8_t *               pbuf,
                                                size_t                  len )
{
	uint32_t        wr_bytes        =   0;


        while( len > 0 )
        {
                p->fresult      =   f_write( &p->file_log, pbuf, len, (void *) &wr_bytes );

                if( p->fresult  == FR_OK )
                {
                        if( wr_bytes < len )
                        {
                                //APP_TRACE( "f_write() - len: %d, wr_bytes: %d\n", len, wr_bytes );
                        }
                        else
                        {
                                //f_sync( &p->file_log );
                        }

                        len     -=  wr_bytes;
                        pbuf    +=  wr_bytes;
                }
                else
                {
                        //APP_TRACE_FF( "f_write() = ", p->fresult );
                        //APP_TRACE( "pbuf: %0X\n", pbuf );
                        //APP_TRACE( "len: %d\n", len );
                        //APP_TRACE( "wr_bytes: %d\n", wr_bytes );
                        break;
                }
        }

        if( p->fresult == FR_OK )
        {
                f_sync( &p->file_log );
        }
}

FRESULT storage_sts_get(                        storage_t *             p )
{
        return( p->fresult );
}
