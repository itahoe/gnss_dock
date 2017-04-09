/**
 * The "time" function returns the current calendar time.  (time_t)-1
 * should be returned if the calendar time is not available.  The time
 * is measured in seconds since the first of January 1970.
 *
 */

#include <time.h>

extern	time_t	time_dat;

#pragma module_name = "?time"

__time32_t (__time32)(__time32_t *t)
{
	return time_dat;
}
