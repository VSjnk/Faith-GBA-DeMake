#include <errno.h>

#define FILE_HOOK_TEMPLATE(hook_name, arguments, return_value) \
   file_hook_t *hook = get_file_hook(file); \
   \
   if (hook) { \
      if (hook->hook_name) { \
         return hook->hook_name arguments; \
      } \
      else { \
         errno = ENOSYS; \
      } \
   } \
   else { \
      errno = EBADF; \
   } \
   \
   return return_value

#include "agb-hooks.h"



