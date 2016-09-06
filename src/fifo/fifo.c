/**
  * @file    fifo.c
  * @brief   FIFO services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include	"fifo.h"


/**
  * @brief
  */
void fifo_init(                                 fifo_t *        p,
                                                uint8_t *       data,
                                        const   size_t          size )
{
        p->data         =   data;
        p->size         =   size;
        p->head         =   0;
        p->tile         =   0;
}

/**
  * @brief
  */
void fifo_flush(                                fifo_t *        p )
{
        p->head         =   0;
        p->tile         =   0;
}

/**
  * @brief
  */
size_t fifo_read(                               fifo_t *        p,
                                                uint8_t *       data,
                                                size_t          size )
{
        size_t          count   =   0;


        while( p->tile != p->head )
        {
                if( count >= size )
                {
                        break;
                }

                *data++         =   p->data[ p->tile++ ];
                p->tile         =   p->tile < p->size ? p->tile : 0;
                count++;
        }

        return( count );
}

/**
  * @brief
  */
size_t fifo_write(                              fifo_t *        p,
                                        const   uint8_t *       data,
                                                size_t          size )
{
        while( size-- )
        {
                p->data[ p->head++ ]    =   *data++;
                p->head                 =   p->head < p->size ? p->head : 0;
        }

        return( 0 );
}
