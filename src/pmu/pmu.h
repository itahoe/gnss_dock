/**
  * @file    pmu.h
  * @brief   Power Management Unit
  * @author  Igor T. <research.tahoe@gmail.com>
  */

#ifndef  PMU_H
#define  PMU_H

typedef	enum pmu_sts_s
{
	PMU_STS_VBUS,
	PMU_STS_CHRG,
	PMU_STS_BATT_FULL,
	PMU_STS_BATT_LOW,
} pmu_sts_t;

typedef	enum pmu_ctl_s
{
	PMU_CTL_LDO,
	PMU_CTL_MCU_OFF,
} pmu_ctl_t;

/*
typedef	struct	pmu_s
{
	pmu_sts_t               status;
} pmu_t;
*/

void	    pmu_init( void );

pmu_sts_t   pmu_sts_get( void );

void	    pmu_ctl(                const   pmu_ctl_t           ctl,
                                    const   bool                bval );

#endif	//PMU_H
