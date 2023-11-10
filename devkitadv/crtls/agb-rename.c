#include "hooks-internal.h"

int _rename (const char *old, const char *new)
{
   filesystem_hook_t *old_hook = get_filesystem_hook(old);
   filesystem_hook_t *new_hook = get_filesystem_hook(new);

   if (old_hook && new_hook) {
      if (old_hook == new_hook) {
         if (old_hook->rename) {
            return old_hook->rename(old, new);
         }
         else {
            errno = ENOSYS;
         }
      }
      else {
         errno = EXDEV;
      }
   }
   else {
      errno = EINVAL;
   }

   return -1;
}



