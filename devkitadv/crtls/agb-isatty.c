#include "hooks-internal.h"

int isatty (int file)
{
   FILE_HOOK_TEMPLATE(close, (file), 0);
}



