/*
 public api
 */

#ifndef AVUTIL_TIME_H
#define AVUTIL_TIME_H

#include <stdint.h>

/**
 * Get the current time in microseconds. 微秒 1s=10^3ms(毫秒)=10^6μs(微秒)
 */
int64_t av_gettime(void);

/**
 * Get the current time in microseconds since some unspecified starting point.
 * On platforms that support it, the time comes from a monotonic clock
 * This property makes this time source ideal for measuring relative time.
 * The returned values may not be monotonic on platforms where a monotonic
 * clock is not available.
 */
int64_t av_gettime_relative(void);

/**
 * Indicates with a boolean result if the av_gettime_relative() time source
 * is monotonic.
 */
int av_gettime_relative_is_monotonic(void);

/**
 * Sleep for a period of time.  Although the duration is expressed in
 * microseconds, the actual delay may be rounded to the precision of the
 * system timer.
 *
 * @param  usec Number of microseconds to sleep.
 * @return zero on success or (negative) error code.
 微秒
 */
int av_usleep(unsigned usec);

#endif /* AVUTIL_TIME_H */
