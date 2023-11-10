#include "hooks-internal.h"

int _stat (const char *path, struct stat *st)
{
   filesystem_hook_t *hook = get_filesystem_hook(path);

   if (hook) {
      if (hook->stat) {
         return hook->stat(path, st);
      }
      else {
         errno = ENOSYS;
      }
   }
   else {
      errno = EINVAL;
   }

   return -1;
}



