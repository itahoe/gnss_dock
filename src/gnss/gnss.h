/**
  * @file    gnss.h
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  GNSS_H
#define  GNSS_H


#include "fifo.h"
#include "nmea.h"
#include "config.h"

typedef	struct	gnss_data_s
{
	size_t                  len;
	size_t                  pingpong;
	uint8_t                 buf[2][ NMEA_STRLEN_MAX_OCT ];
	uint8_t *               str;
} gnss_data_t;

typedef	struct	gnss_s
{
	nmea_t                  nmea;
	gnss_data_t             recv;
	//gnss_data_t             xmit;
} gnss_t;


void gnss_init(                                 gnss_t *                gnss );

void gnss_ser1_recv_start(                      uint8_t *               data,
                                                size_t                  size );

void gnss_ser2_recv_start(                      uint8_t *               data,
                                                size_t                  size );

void gnss_ser3_recv_start(                      uint8_t *               data,
                                                size_t                  size );

bool gnss_recv(                                 gnss_t *                gnss,
                                        const   char                    c );

void gnss_uart_rx_hook(                         fifo_t *                p,
                                                size_t                  size );

size_t gnss_ser3_xmit(                          uint8_t *               data,
                                                size_t                  size );

size_t gnss_ser2_xmit(                          uint8_t *               data,
                                                size_t                  size );

void gnss_time_sync(                            gnss_t *                gnss,
                                                time_t *                time_dat );

#endif	//GNSS_H
