/**
  * @file    nmea.h
  * @brief   NMEA service
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  NMEA_H
#define  NMEA_H

#include	<stdbool.h>
#include	<stdint.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	"config.h"
#include	"nmea_x.h"

#define	NMEA_STRLEN_MAX_OCT             (80 + CFG_NMEA_STRLEN_EXTRA_OCT)

typedef	enum nmea_nsid_s
{
	NMEA_NSID_INVALID,
	NMEA_NSID_MULTI,
	NMEA_NSID_GPS,
	NMEA_NSID_GLONASS,
	NMEA_NSID_GALILEO,
} nmea_nsid_t;

typedef	enum nmea_fix_s
{
	NMEA_FIX_ERROR          =   0,
	NMEA_FIX_NONE           =   '0',
	NMEA_FIX_SPS            =   '1', //Standard Positioning Service
	NMEA_FIX_DGPS           =   '2', //Differential GPS fix
	NMEA_FIX_PPS            =   '3', //Precise Positioning Service
	NMEA_FIX_RTK_INT        =   '4', //RTK Fixed Integer
	NMEA_FIX_RTK_FLT        =   '5', //RTK Float
	NMEA_FIX_DR             =   '6', //Dead Reckoning
	NMEA_FIX_MANUAL         =   '7', //Manual input mode
	NMEA_FIX_SIM            =   '8', //Simulation mode
} nmea_fix_t;

typedef	enum nmea_sel_mode_s
{
	NMEA_SEL_MODE_ERROR     =   0,
	NMEA_SEL_MODE_MANUAL    =   'M',
	NMEA_SEL_MODE_AUTO      =   'A',
} nmea_sel_mode_t;

typedef	enum nmea_fix_mode_s
{
	NMEA_FIX_MODE_ERROR     =   0,
	NMEA_FIX_MODE_NONE      =   '1',
	NMEA_FIX_MODE_2D        =   '2',
	NMEA_FIX_MODE_3D        =   '3',
} nmea_fix_mode_t;

typedef	enum nmea_sts_s
{
	NMEA_STS_ERROR          =   0,
	NMEA_STS_VALID          =   'A',
	NMEA_STS_INVALID        =   'V',
} nmea_sts_t;

typedef	struct	nmea_gga_s
{
	struct tm	        tm_dat;
	float                   latitude;
	float                   longitude;
	nmea_fix_t              fix;
	int                     sats_used;
	float                   hdop;
	float                   altitude;
	float                   geoidal_separation;
	float                   dgps_data_age_sec;
	int                     dgps_station_id;
} nmea_gga_t;

typedef	struct	nmea_gsa_s
{
	nmea_sel_mode_t         sel_mode;
	nmea_fix_mode_t         fix_mode;
	int                     sat_id[12];
	float                   pdop;
	float                   hdop;
	float                   vdop;
} nmea_gsa_t;

typedef	struct	nmea_gsv_s
{
	int                     msgs_total;
	int                     msg_num;
	int                     sats_in_view;
	int                     sat_num;
	int                     elevation_deg;
	int                     azimuth_deg;
	int                     snr_db;
} nmea_gsv_t;

typedef struct tm	nmea_tm_t;

typedef	struct	nmea_rmc_s
{
	nmea_sts_t              status;
	float                   latitude;
	float                   longitude;
	float                   speed;
	float                   track_angle;
	float                   magnetic_variation;
	struct tm	        tm_dat;
} nmea_rmc_t;

typedef enum    nmea_pckt_type_s
{
        NMEA_PCKT_TYPE_NONE,
        NMEA_PCKT_TYPE_GGA,
        NMEA_PCKT_TYPE_GSA,
        NMEA_PCKT_TYPE_GSV,
        NMEA_PCKT_TYPE_RMC,
        NMEA_PCKT_TYPE_X,
} nmea_pckt_type_t;

typedef	enum nmea_state_s
{
	NMEA_STATE_IDLE,
	NMEA_STATE_RECV,
	NMEA_STATE_CHKSUM,
} nmea_state_t;

typedef	struct	nmea_s
{
        uint8_t                 chksum;
	size_t                  chksum_errors;
        nmea_state_t            state;
	size_t                  recv_total;
	nmea_nsid_t             nsid;
	nmea_gga_t              gga;
	nmea_gsa_t              gsa;
	nmea_gsv_t              gsv;
	nmea_rmc_t              rmc;
        nmea_x_t                x;
} nmea_t;


static
void nmea_str2int(                          int *               i,
                                    const   char *              str )
{
	*i      =   strtol( str, NULL, 10 );
}

static
void nmea_str2flt(                          float *             f,
                                    const   char *              str )
{
	*f      =   strtof( str, NULL );
}

static
void nmea_str2time(                         struct tm *         p,
	                            const   char *              str )
{
	p->tm_hour  =   ( *str++ - '0' ) * 10;
	p->tm_hour  +=  ( *str++ - '0' ) * 1;

	p->tm_min   =   ( *str++ - '0' ) * 10;
	p->tm_min   +=  ( *str++ - '0' ) * 1;

	p->tm_sec   =   ( *str++ - '0' ) * 10;
	p->tm_sec   +=  ( *str++ - '0' ) * 1;
}

static
void nmea_str2date(                         struct tm *         p,
	                            const   char *              str )
{
	p->tm_mday  =   ( *str++ - '0' ) * 10;
	p->tm_mday  +=  ( *str++ - '0' ) * 1;

	p->tm_mon   =   ( *str++ - '0' ) * 10;
	p->tm_mon   +=  ( *str++ - '0' ) * 1;

	p->tm_year  =   ( *str++ - '0' ) * 10;
	p->tm_year  +=  ( *str++ - '0' ) * 1;
}

void nmea_init(     nmea_t * nmea );

size_t nmea_chksum( uint8_t *chksum, const char *str, const char eol );

bool    nmea_recv(                              nmea_t *        nmea,
                                        const   char *          str );

nmea_pckt_type_t        nmea_recv_gga( nmea_gga_t * gga, const char * str );
nmea_pckt_type_t        nmea_recv_gsa( nmea_gsa_t * gsa, const char * str );
nmea_pckt_type_t        nmea_recv_gsv( nmea_gsv_t * gsv, const char * str );
nmea_pckt_type_t        nmea_recv_rmc( nmea_rmc_t * rmc, const char * str );

nmea_pckt_type_t        nmea_getc(     nmea_t *            p,
                                       char                c );

#endif	//NMEA_H
