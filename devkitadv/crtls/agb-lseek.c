#include "hooks-internal.h"

int _lseek (int file, int ptr, int dir)
{
   FILE_HOOK_TEMPLATE(lseek, (file, ptr, dir), -1);
}



