/**
  * @file    gnss_bridge.c
  * @brief   UART to UART bridge
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "bsp_gnss.h"
#include "gnss.h"


/**
 * @brief GNSS initialization
 */
void gnss_bridge_init(                           gnss_t *            p )
{
	bsp_gnss_bridge_init();

	memset( p, 0, sizeof( gnss_t ) );
}
