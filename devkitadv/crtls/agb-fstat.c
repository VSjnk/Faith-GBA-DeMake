#include "hooks-internal.h"

int _fstat (int file, struct stat * st)
{
   FILE_HOOK_TEMPLATE(fstat, (file, st), -1);
}



