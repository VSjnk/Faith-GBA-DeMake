#include <sys/times.h>
#include <errno.h>

clock_t _times (struct tms * tp)
{
  clock_t timeval;

  timeval = (clock_t) -1;

  if (tp) {
      tp->tms_utime  = timeval;	/* user time */
      tp->tms_stime  = 0;	/* system time */
      tp->tms_cutime = 0;	/* user time, children */
      tp->tms_cstime = 0;	/* system time, children */
  }

  errno = ENOSYS;

  return timeval;
}


