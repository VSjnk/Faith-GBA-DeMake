#include "hooks-internal.h"

#include <stdarg.h>

int _open (const char * path, int flags, ... )
{
   filesystem_hook_t *hook;
   va_list ap;

   va_start(ap, flags);

   hook = get_filesystem_hook(path);

   if (hook) {
      if (hook->open) {
         int retval = hook->open(path, flags, va_arg(ap, int));
         va_end(ap);
         return retval;
      }
      else {
         errno = ENOSYS;
      }
   }
   else {
      errno = EINVAL;
   }

   va_end(ap);

   return -1;
}



