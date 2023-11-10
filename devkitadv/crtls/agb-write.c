#include "hooks-internal.h"

int _write (int file, char * ptr, int len)
{
   FILE_HOOK_TEMPLATE(write, (file, ptr, len), -1);
}



