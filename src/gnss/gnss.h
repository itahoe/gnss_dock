/**
  * @file    gnss.h
  * @brief   GNSS service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  GNSS_H
#define  GNSS_H


#include	"bsp_gnss.h"
#include	"nmea.h"
#include	"config.h"


typedef	enum gnss_ctl_s
{
	GNSS_CTL_RECV_START,
	GNSS_CTL_RECV_STOP,
	GNSS_CTL_XMIT_START,
	GNSS_CTL_XMIT_STOP,
} gnss_ctl_t;


typedef	struct	gnss_xfer_s
{
	size_t                  len;
	size_t                  pingpong;
	char                    buf[2][ NMEA_STRLEN_MAX_OCT ];
	//char                    buf[ NMEA_STRLEN_MAX_OCT ];
	char *                  str;
} gnss_xfer_t;

typedef	struct	gnss_s
{
	nmea_t                  nmea;
	gnss_xfer_t             recv;
	gnss_xfer_t             xmit;
} gnss_t;


void	gnss_time_sync(                     gnss_t *            gnss,
                                            time_t *            time_dat );
void	gnss_ui_set(                        gnss_t *            gnss );

void	gnss_init(                          gnss_t *            gnss );

void	gnss_ctl(                           gnss_ctl_t          ctl );

void	gnss_recv(                          gnss_t *            gnss,
                                    const   uint8_t *           str,
	                                    size_t              size );

#endif	//GNSS_H
