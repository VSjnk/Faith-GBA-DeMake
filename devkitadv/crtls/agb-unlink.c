#include "hooks-internal.h"

int _unlink (const char* path)
{
   filesystem_hook_t *hook = get_filesystem_hook(path);

   if (hook) {
      if (hook->unlink) {
         return hook->unlink(path);
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



