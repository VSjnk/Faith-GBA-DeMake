#include "hooks-internal.h"

int _read (int file, char * ptr, int len)
{
   FILE_HOOK_TEMPLATE(read, (file, ptr, len), -1);
}



