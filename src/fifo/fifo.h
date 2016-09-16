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
        size_t                  total_overruns;
        size_t                  total_overcomes;
        size_t                  total_data;
        #endif //NDEBUG
} fifo_t;

void fifo_write(                                fifo_t *        p,
                                                uint8_t *       data,
                                                size_t          size );


#endif	//FIFO_H
