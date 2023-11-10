#include <errno.h>

int _system (const char *s)
{
  if (!s) return 0;

  errno = ENOSYS;
  return -1;
}



