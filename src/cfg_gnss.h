/**
  * @file    cfg_gnss.h
  * @brief   Config - GNSS
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  CFG_GNSS_H
#define  CFG_GNSS_H


//#define CFG_GNSS_BLCK_SIZE_OCT          4096
#define CFG_GNSS_BLCK_SIZE_OCT          8192

#define CFG_GNSS_UART2_BLCK_SIZE_OCT	4096
#define CFG_GNSS_UART3_BLCK_SIZE_OCT	4096

#define	CFG_GNSS_MSG_KEY0L              "PNVGCFG,W"
#define	CFG_GNSS_MSG_KEY0S              "PNVGRZA,2,9600,7" //UART2 - bluetooth
//#define	CFG_GNSS_MSG_KEY0S              "PNVGRZA,3,115200,1"  //UART1 - NMEA
//#define	CFG_GNSS_MSG_KEY0S              "PNVGNME,2,7,0"


#define	CFG_GNSS_MSG_KEY1L              ""
#define	CFG_GNSS_MSG_KEY1S              "PNVGRTK,MODE,3,AVGTIME,3"


#endif	//CFG_GNSS_H
