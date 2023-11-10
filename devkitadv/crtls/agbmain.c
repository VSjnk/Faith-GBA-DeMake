#include <stdlib.h>



void _init(void);
void _fini(void);

__attribute__ ((long_call)) int main(int argc, char *argv[], char *environ[]);

/* The following variables could have been provided in seperate object files
   so that the user could override them.  However, it is possible to save
   a few bytes by declaring them weak.  The pointers will then be defined as
   0 by the linker if the user does not provide them.  __argc is given a
   definition, because it is not a pointer.  Although dereferencing 0 would
   not be a big deal on the GBA, its value would be undefined */

__attribute__ ((weak, section(".cartrom.data"))) int __argc = 0;
__attribute__ ((weak)) extern char **__argv;
__attribute__ ((weak)) extern char **__environ;

void AgbMain(void)
{
   atexit(_fini);
   _init();
   exit(main(__argc, __argv, __environ));
}



