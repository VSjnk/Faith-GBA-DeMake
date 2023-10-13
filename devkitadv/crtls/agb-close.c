#include "hooks-internal.h"

int _close (int file)
{
   FILE_HOOK_TEMPLATE(close, (file), -1);
}



