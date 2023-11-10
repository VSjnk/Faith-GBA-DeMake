#include <errno.h>
#include <sys/time.h>

int _gettimeofday (__attribute__((unused)) struct timeval * tp, __attribute__((unused)) struct timezone * tzp)
{
  errno = ENOSYS;
  return -1;
}



