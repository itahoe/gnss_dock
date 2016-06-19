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
	bool            sts_charge      =   bsp_pmu_sts_charge_get();
	bool            sts_pgood       =   bsp_pmu_sts_pgood_get();
	pmu_sts_t       status;


	status          =   (sts_pgood && sts_charge) ? PMU_STS_CHRG :
	                    (sts_pgood && !sts_charge) ? PMU_STS_VBUS :
	                    !sts_pgood ? PMU_STS_BATT_FULL : PMU_STS_BATT_LOW;

	return( status );
}
