#include "hooks-internal.h"

static struct filesystem_hook_t **sysbuf;
static int                        sysbuf_size;

void set_filesystem_hook_buffer(filesystem_hook_t **buf, int buf_size)
{
   sysbuf = buf;
   sysbuf_size = buf_size;
}

filesystem_hook_t *get_filesystem_hook(const char* path)
{
   int i;

   if (sysbuf) {
      for (i = 0; i < sysbuf_size; i++) {
         if (sysbuf[i]) {
            if (sysbuf[i]->valid_path) {
               if (sysbuf[i]->valid_path(path)) {
                  return sysbuf[i];
               }
            }
         }
      }
   }

   return 0;
}



