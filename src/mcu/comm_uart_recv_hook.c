/**
  * @file    app_uart_recv_hook.c
  * @brief   UART Recieve Hook
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include "comm.h"


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
bool comm_uart_recv_hook(                   comm_pipe_inp_t *       p )
{
        bool            resp            =   false;
        uint32_t        ndtr            =   p->dma_get();
        uint8_t *       head            =   p->data + (p->blck_size - ndtr);


        if( head != p->head )
        {
                if( head < p->head )
                {
                        p->size         =  (p->data + p->blck_size) - p->head;
                        head            =   p->data;
                }
                else
                {
                        p->size         =  head - p->head;
                }

                p->tile         =   p->head;
                p->head         =   head;
                p->ndtr         =   ndtr;
                resp            =   true;
        }

        return( resp );
}
