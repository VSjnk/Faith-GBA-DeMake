#include <errno.h>

int _kill (__attribute__((unused)) int n, __attribute__((unused)) int m)
{
  /* function not implemented */
  errno = ENOSYS;
  return -1;
}



