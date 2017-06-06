/**
  * @file    nvg_getc.c
  * @brief   NVG get single char
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include <stdio.h>
#include "nvg.h"
#include "nmea.h"


/**
 * @brief recieve single char
 */
void nvg_resp(                                  nvg_t *                 p,
                                        const   nvg_log_mode_t          mode )
{
        uint8_t                 chksum;

        p->size                 =   snprintf(   p->str_out,
                                                sizeof(p->str_out),
                                                "$PNVGLOG,MODE,%d*",
                                                mode );

        nmea_chksum( &chksum, p->str_out+1, '*' );

        p->size                 +=  snprintf(   p->str_out + p->size,
                                                sizeof(p->str_out) - p->size,
                                                "%02X\r\n",
                                                chksum );
}

