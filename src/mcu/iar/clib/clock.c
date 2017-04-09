/*
 * The "clock" function should return the processor time used by the
 * program from some implementation-defined start time.  The value
 * should be such that if divided by the macro CLOCKS_PER_SEC the
 * result should yield the time in seconds.
 *
 * The value "(clock_t)-1" means that the processor time is not
 * available.
 *
 */

#include <time.h>

_STD_BEGIN

#pragma module_name = "?clock"

extern	clock_t clk_count;

clock_t (clock)(void)
{
	return( clk_count );
}

_STD_END
