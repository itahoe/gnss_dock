/**
  * @file    comm.h
  * @brief   Comm header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef COMM_H
#define COMM_H

#include <stdbool.h>
#include "config.h"


typedef enum    comm_msg_e
{
        COMM_MSG_INVALID,
        COMM_MSG_CLI,
        COMM_MSG_,
} comm_msg_t;

typedef bool        ( * comm_init_t )(  const   size_t          baudrate );

typedef bool        ( * comm_open_t )(          uint8_t *       data,
                                                size_t          size );
typedef bool        ( * comm_close_t )( void );

typedef uint32_t    ( * comm_dma_get_t )( void );

typedef	struct	comm_pipe_inp_s
{
        comm_open_t             open;
        comm_close_t            close;
        comm_dma_get_t          dma_get;
        uint32_t                ndtr;
        //uint32_t                dma_ndtr_prev;
        uint8_t *               head;
        uint8_t *               tile;
        size_t                  size;
        size_t                  blck_size;
        uint8_t *               data;
} comm_pipe_inp_t;

typedef	struct	comm_pipe_out_s
{
        comm_open_t             open;
        comm_close_t            close;
        comm_dma_get_t          dma_get;
        uint32_t                ndtr;
        //uint32_t                dma_ndtr_prev;
        uint8_t *               head;
        uint8_t *               tile;
        size_t                  size;
        size_t                  blck_size;
        uint8_t *               data;
} comm_pipe_out_t;

typedef	struct	comm_s
{
        comm_init_t             init;
        comm_open_t             xmit;
        //size_t                  baudrate;
        comm_pipe_inp_t         recv;
        //comm_pipe_out_t         xmit;
} comm_t;


bool comm_uart_recv_hook(                       comm_pipe_inp_t *       p );


#endif	//COMM_H
