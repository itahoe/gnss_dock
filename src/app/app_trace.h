/**
  * @file    app_trace.h
  * @brief   App - Trace/Debug
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef APP_TRACE_H
#define APP_TRACE_H

#include <stdio.h>
#include "ff.h"

static
void app_trace_error(const uint32_t error_code,
                     const uint32_t line_num,
                     const char *file_name)
{
	printf("\n\rERROR: 0x%0X,\t%s, line %d\r\n",
	       error_code,
	       file_name,
	       line_num);
}

static
void	app_trace_ff_res( int i )
{
	switch( i )
	{
		case FR_OK:                     printf( "FR_OK\n"                  ); break; /* (0) Succeeded */
		case FR_DISK_ERR:               printf( "FR_DISK_ERR\n"            ); break; /* (1) A hard error occurred in the low level disk I/O layer */
		case FR_INT_ERR:                printf( "FR_INT_ERR\n"             ); break; /* (2) Assertion failed */
		case FR_NOT_READY:              printf( "FR_NOT_READY\n"           ); break; /* (3) The physical drive cannot work */
		case FR_NO_FILE:                printf( "FR_NO_FILE\n"             ); break; /* (4) Could not find the file */
		case FR_NO_PATH:                printf( "FR_NO_PATH\n"             ); break; /* (5) Could not find the path */
		case FR_INVALID_NAME:           printf( "FR_INVALID_NAME\n"        ); break; /* (6) The path name format is invalid */
		case FR_DENIED:                 printf( "FR_DENIED\n"              ); break; /* (7) Access denied due to prohibited access or directory full */
		case FR_EXIST:                  printf( "FR_EXIST\n"               ); break; /* (8) Access denied due to prohibited access */
		case FR_INVALID_OBJECT:         printf( "FR_INVALID_OBJECT\n"      ); break; /* (9) The file/directory object is invalid */
		case FR_WRITE_PROTECTED:        printf( "FR_WRITE_PROTECTED\n"     ); break; /* (10) The physical drive is write protected */
		case FR_INVALID_DRIVE:          printf( "FR_INVALID_DRIVE\n"       ); break; /* (11) The logical drive number is invalid */
		case FR_NOT_ENABLED:            printf( "FR_NOT_ENABLED\n"         ); break; /* (12) The volume has no work area */
		case FR_NO_FILESYSTEM:          printf( "FR_NO_FILESYSTEM\n"       ); break; /* (13) There is no valid FAT volume */
		case FR_MKFS_ABORTED:           printf( "FR_MKFS_ABORTED\n"        ); break; /* (14) The f_mkfs() aborted due to any parameter error */
		case FR_TIMEOUT:                printf( "FR_TIMEOUT\n"             ); break; /* (15) Could not get a grant to access the volume within defined period */
		case FR_LOCKED:                 printf( "FR_LOCKED\n"              ); break; /* (16) The operation is rejected according to the file sharing policy */
		case FR_NOT_ENOUGH_CORE:        printf( "FR_NOT_ENOUGH_CORE\n"     ); break; /* (17) LFN working buffer could not be allocated */
		case FR_TOO_MANY_OPEN_FILES:	printf( "FR_TOO_MANY_OPEN_FILES\n" ); break; /* (18) Number of open files > _FS_SHARE */
		case FR_INVALID_PARAMETER:      printf( "FR_INVALID_PARAMETER\n"   ); break; //(19) Given parameter is invalid
		default:                        printf( "FR_XXX\n"                 ); break;
	}
}


#ifdef  NDEBUG

# define APP_TRACE_INIT()
# define APP_TRACE(...)
# define APP_TRACE_ERROR(ERR_CODE)
# define APP_TRACE_FF_RES(...)

#else

# define APP_TRACE_INIT()
# define APP_TRACE(...)                 printf(__VA_ARGS__)
# define APP_TRACE_ERROR(ERR_CODE)      app_trace_error((ERR_CODE), __LINE__, __FILE__)
# define APP_TRACE_FF_RES(ERR_CODE)     app_trace_ff_res(ERR_CODE)

#endif

#endif	//APP_TRACE_H
