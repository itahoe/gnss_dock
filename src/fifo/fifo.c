/**
  * @file    fifo.c
  * @brief   FIFO services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"fifo.h"


/**
 * @brief GNSS xmit
 */
void fifo_write(                                fifo_t *        p,
                                                uint8_t *       data,
                                                size_t          count )
{
	while( count-- )
	{
		p->data[ p->head++ ]    =   *data++;

		if( p->head >= p->size )
		{
			p->head                 =   0;
		}
	}
}

