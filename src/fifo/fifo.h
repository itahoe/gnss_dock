/**
  * @file    fifo.h
  * @brief   FIFO services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef	FIFO_H
#define	FIFO_H


#include <stdint.h>
#include <stddef.h>


typedef	struct	fifo_s
{
        uint8_t *       data;
        size_t          size;
        size_t          head;
        size_t          tile;
}	fifo_t;


void fifo_init(                                 fifo_t *        p,
                                                uint8_t *       data,
                                        const   size_t          size );

void fifo_flush(                                fifo_t *        p );

size_t fifo_read(                               fifo_t *        p,
                                                uint8_t *       data,
                                                size_t          size );

size_t fifo_write(                              fifo_t *        p,
                                        const   uint8_t *       data,
                                                size_t          size );

#endif	//FIFO_H
