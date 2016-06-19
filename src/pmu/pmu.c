/**
  * @file    pmu.h
  * @brief   Power Management Unit
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#include	"bsp_pmu.h"
#include	"pmu.h"

/**
 * @brief PMU Initialization.
 */
void	pmu_init( void )
{
	bsp_pmu_init();
}
