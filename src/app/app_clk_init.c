/**
  * @file    app_clk_init.c
  * @brief   Configure MCU clock tree
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include <stdio.h>
#include "app.h"


/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
HAL_StatusTypeDef app_clk_init( void )
{
        HAL_StatusTypeDef       resp            =   HAL_OK;
        RCC_OscInitTypeDef      osc;
        RCC_ClkInitTypeDef      clk;

        /* Enable Power Control clock */
        __HAL_RCC_PWR_CLK_ENABLE();

        /*      The voltage scaling allows optimizing the power consumption when the device is
                clocked below the maximum system frequency, to update the voltage scaling value
                regarding system frequency refer to product datasheet.  */
        __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

        /* Enable HSE Oscillator and activate PLL with HSE as source */
        osc.OscillatorType      =   RCC_OSCILLATORTYPE_HSE;
        osc.HSEState            =   RCC_HSE_ON;
        osc.PLL.PLLState        =   RCC_PLL_ON;
        osc.PLL.PLLSource       =   RCC_PLLSOURCE_HSE;
        osc.PLL.PLLM            =   16;
        //osc.PLL.PLLN            =   360;
        osc.PLL.PLLN            =   336;
        osc.PLL.PLLP            =   RCC_PLLP_DIV2;
        osc.PLL.PLLQ            =   7;
        osc.PLL.PLLR            =   2;
        resp                    =   HAL_RCC_OscConfig( &osc );

        if(resp != HAL_OK)
        {
                return( resp );
        }

        /* activate the OverDrive to reach the 180 Mhz Frequency */
        resp                    =   HAL_PWREx_EnableOverDrive();
        if(resp != HAL_OK)
        {
                return( resp );
        }

        /*      Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
                clocks dividers */
        clk.ClockType           =   (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
        clk.SYSCLKSource        =   RCC_SYSCLKSOURCE_PLLCLK;
        clk.AHBCLKDivider       =   RCC_SYSCLK_DIV1;
        clk.APB1CLKDivider      =   RCC_HCLK_DIV4;
        clk.APB2CLKDivider      =   RCC_HCLK_DIV2;

        resp                    =   HAL_RCC_ClockConfig( &clk, FLASH_LATENCY_5 );

        return( resp );
}

