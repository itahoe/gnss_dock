/**
  * @file    storage.h
  * @brief   Storage
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  STORAGE_H
#define  STORAGE_H

#include <stdint.h>
#include <stdbool.h>
#include "ff.h"
#include "config.h"


#define STORAGE_FEXT_MAX        8


typedef	union	storage_sts_u
{
	uint16_t				whole;
	struct
	{
		bool                    enable          :    1;
		bool                    ready1          :    1;
		bool                    ready2          :    1;
		bool                                    :    0;
	};
} storage_sts_t;

typedef struct  storage_s
{
	storage_sts_t   sts;
	FIL             file_log;
	char            fname[ _MAX_LFN ];
	char            fext[ STORAGE_FEXT_MAX ];
	FRESULT         fresult;
} storage_t;


void storage_init( void );

void storage_close(                             storage_t *             p );

bool storage_open(                              storage_t *             p );

void storage_write(                             storage_t *             p,
                                        const   uint8_t *               pbuf,
                                                size_t                  len );

int storage_name_compose(                       char *                  str,
                                        const   char *			ext,
                                                size_t                  max_len );

FRESULT storage_sts_get(                        storage_t *             p );


#endif	//STORAGE_H
