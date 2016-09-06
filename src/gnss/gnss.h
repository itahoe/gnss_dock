/**
  * @file    gnss.h
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  GNSS_H
#define  GNSS_H


#include	"nmea.h"
#include	"config.h"

typedef	struct	gnss_data_s
{
	size_t                  len;
	size_t                  pingpong;
	uint8_t                 buf[2][ NMEA_STRLEN_MAX_OCT ];
	uint8_t *               str;
} gnss_data_t;

#pragma pack(4)
typedef	struct	gnss_fifo_s
{
        //uint8_t                 data[ CFG_GNSS_BLCK_SIZE_OCT ];
        uint8_t *               data;
	size_t                  size;
	size_t                  head;
	size_t                  tile;
	bool                    overcome;

	#ifndef  NDEBUG
        size_t                  total_overruns;
        size_t                  total_overcomes;
        size_t                  total_data;
	#endif //NDEBUG

} gnss_fifo_t;
#pragma pack()

typedef	struct	gnss_s
{
	nmea_t                  nmea;
	gnss_data_t             recv;
	gnss_data_t             xmit;
} gnss_t;


void gnss_time_sync(                            gnss_t *        gnss,
                                                time_t *        time_dat );

void gnss_fifo_write(                           gnss_fifo_t *   p,
                                                uint8_t *       data,
                                                size_t          count );

void gnss_ui_set(                               gnss_t *        gnss );

void gnss_init(                                 gnss_t *        gnss );

void gnss_recv_start(                           uint8_t *       data,
                                                size_t          size );

void gnss_send(                                 gnss_t *        gnss,
                                        const   char *          str );

void gnss_read(                                 gnss_t *        gnss,
                                        const   uint8_t *       str,
                                                size_t          size );

void gnss_uart_rx_hook(                         gnss_fifo_t *   p );


#endif	//GNSS_H
