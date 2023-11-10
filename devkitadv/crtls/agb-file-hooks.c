#include "hooks-internal.h"

static struct file_hook_t **fbuf;
static int                  fbuf_size;

void set_file_hook_buffer(file_hook_t **buf, int buf_size)
{
   fbuf = buf;
   fbuf_size = buf_size;
}

file_hook_t *get_file_hook(int file)
{
   if (fbuf && file < fbuf_size) {
      return fbuf[file];
   }
   else {
      return 0;
   }
}



