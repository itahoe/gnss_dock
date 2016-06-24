/**
  * @file    pmu_sts.h
  * @brief   Power Management Unit - Status
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_pmu.h"
#include	"pmu.h"

/**
 * @brief PMU Status
 */
pmu_sts_t   pmu_sts_get( void )
{
	bool            sts_pgood       =   bsp_pmu_sts_pgood_get();
	bool            sts_charge      =   bsp_pmu_sts_charge_get();
	pmu_sts_t       status;


	if( sts_charge )
	{
		status          =   PMU_STS_CHRG;
	}
	else
	{
		if( sts_pgood )
		{
			status          =   PMU_STS_VBUS ;
		}
		else
		{
			status          =   PMU_STS_BATT_FULL;
		}
	}

	return( status );
}
