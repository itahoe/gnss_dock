/**
  * @file    fifo.h
  * @brief   FIFO services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef	FIFO_H
#define	FIFO_H


#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


typedef	struct	fifo_s
{
        uint8_t *               data;
        size_t                  size;
        size_t                  head;
        size_t                  tile;
        size_t                  ndtr;
        bool                    overcome;

        #ifndef NDEBUG
        size_t                  total_overrun;
        size_t                  total_overcome;
        size_t                  total_data;
        #endif //NDEBUG
} fifo_t;

static
void fifo_dbg_overcome_increment(               fifo_t *                p )
{
        #ifndef  NDEBUG
        p->total_overcome++;
        #endif //NDEBUG
}

static
void fifo_dbg_overrun_increment(                fifo_t *                p )
{
        #ifndef  NDEBUG
        p->total_overrun++;
        #endif //NDEBUG
}

static
void fifo_dbg_datablcks_increment(              fifo_t *                p,
                                                size_t                  size )
{
        #ifndef  NDEBUG
        p->total_data           +=   size;
        #endif //NDEBUG
}

void fifo_write(                                fifo_t *                p,
                                                uint8_t *               data,
                                                size_t                  size );


#endif	//FIFO_H
