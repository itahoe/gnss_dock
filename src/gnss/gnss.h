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

typedef	uint8_t                 gnss_smbl_t;

typedef	struct	gnss_data_s
{
	size_t                  len;
	size_t                  pingpong;
	gnss_smbl_t             buf[2][ NMEA_STRLEN_MAX_OCT ];
	gnss_smbl_t *           str;
} gnss_data_t;

typedef	struct	gnss_fifo_s
{
        #pragma pack(4)
	gnss_smbl_t             data[ CFG_GNSS_BLCK_SIZE_OCT ];
        #pragma pack()

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

typedef	struct	gnss_s
{
	nmea_t                  nmea;
	gnss_data_t             recv;
	gnss_data_t             xmit;
} gnss_t;


void gnss_time_sync(                            gnss_t *        gnss,
                                                time_t *        time_dat );

void gnss_xmit(                                 gnss_fifo_t *   p,
                                                uint8_t *       data,
                                                size_t          count );

void gnss_ui_set(                               gnss_t *        gnss );

void gnss_init(                                 gnss_t *        gnss );

void gnss_ctl(                                  gnss_ctl_t      ctl );

void gnss_send(                                 gnss_t *        gnss,
                                        const   char *          str );

void gnss_read(                                 gnss_t *        gnss,
                                        const   uint8_t *       str,
                                                size_t          size );

void gnss_uart_rx_hook(                         gnss_fifo_t *   p );


#endif	//GNSS_H
