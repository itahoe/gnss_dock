/**
  * @file    app_task_main.c
  * @brief   Main Task services
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "usbd_cdc_interface.h"
#include "bsp_mcu.h"
#include "gnss.h"
#include "cmsis_os.h"
#include "app.h"
#include "pmu.h"
#include "ui.h"


extern  TaskHandle_t            task_ui;
extern  TaskHandle_t            task_gnss;


void app_task_main(                             void *            argument )
{
        (void) argument;

        pmu_ctl( PMU_CTL_GNSS_LDO_ON, true );


        while( true )
        {
                osThreadSuspend( NULL );
        }

}
