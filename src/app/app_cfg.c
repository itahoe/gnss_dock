/**
  * @file    app_cfg.c
  * @brief   Config
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include "app.h"
#include "nvg.h"


static  RTC_HandleTypeDef       hrtc;


bool app_cfg_init(                              app_cfg_t *             p )
{
        bool            err     =   false;

        __HAL_RCC_PWR_CLK_ENABLE();
        HAL_PWR_EnableBkUpAccess();

        __HAL_RCC_RTC_ENABLE();

        hrtc.Instance                   =   RTC;
/*
        hrtc.Init.HourFormat            =   RTC_HOURFORMAT_24;
        hrtc.Init.AsynchPrediv          =   RTC_ASYNCH_PREDIV;
        hrtc.Init.SynchPrediv           =   RTC_SYNCH_PREDIV;
        hrtc.Init.OutPut                =   RTC_OUTPUT_DISABLE;
        hrtc.Init.OutPutPolarity        =   RTC_OUTPUT_POLARITY_HIGH;
        hrtc.Init.OutPutType            =   RTC_OUTPUT_TYPE_OPENDRAIN;
*/

        if( HAL_RTC_Init( &hrtc ) != HAL_OK)
        {
                err     =   true;
        }

        p->log_mode     =   app_cfg_read( RTC_BKP_DR0 );

        if(     (p->log_mode != NVG_LOG_MODE_3) &&
                (p->log_mode != NVG_LOG_MODE_2) &&
                (p->log_mode != NVG_LOG_MODE_1)    )
        {
                p->log_mode     =   NVG_LOG_MODE_1;
                app_cfg_write( RTC_BKP_DR0, p->log_mode );
        }


        return( err );
}


uint32_t app_cfg_read(                  const   uint32_t                addr )
{
        uint32_t        rslt    =   HAL_RTCEx_BKUPRead( &hrtc, addr );

        return( rslt );
}


void app_cfg_write(                     const   uint32_t                addr,
                                                uint32_t                data )
{
        HAL_RTCEx_BKUPWrite( &hrtc, addr, data );
}

