/**
  * @file    app_cfg.h
  * @brief   App Config Header
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#ifndef APP_CFG_H
#define APP_CFG_H

#include <stdbool.h>
#include "app_trace.h"
#include "config.h"


void app_task_gnss_com2o(                       void *          argument );

void app_com1_recv_idle_isr(                    uint32_t        cnt );
void app_com1_recv_half_isr( void );
void app_com1_recv_full_isr( void );

void app_task_storage(                          void *          argument );
void app_task_cli(                              void *          argument );


#endif	//APP_CFG_H
