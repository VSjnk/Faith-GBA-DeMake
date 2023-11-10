#include <sys/types.h>
#include <errno.h>

register char * stack_ptr asm ("sp");

extern char _end[];             /* defined by the link script */
extern char __heap_limit[];     /* defined by the link script */

extern char __appended_start[]; /* defined by the link script */
extern char __appended_end[];   /* defined by the link script */

__attribute__ ((weak)) extern char *__load_start_appended_ptr;
__attribute__ ((weak)) extern char *__load_break_appended_ptr;

extern void _start();

static char *heap_start;
static char *heap_end;



__attribute__ ((always_inline)) static inline char *fixup_end()
{
   if (_end == __appended_end &&
       &__load_start_appended_ptr != 0 &&
       &__load_break_appended_ptr != 0) {

      return __appended_start + (__load_break_appended_ptr - __load_start_appended_ptr);
   }
   else {
      return _end;
   }
}



__attribute__ ((always_inline)) static inline int check_stack(char *next_start)
{
   if (stack_ptr >= heap_start && stack_ptr < heap_end) {
      return next_start < (stack_ptr - 128); // 128 byte cushion between stack and heap
   }
   else {
      return 1;
   }
}



__attribute__ ((always_inline)) static inline int check_limit(char *next_start)
{
   return next_start < heap_end;
}



void _set_heap(char *new_start, char *new_end)
{
   heap_start = new_start;
   heap_end   = new_end;
}



caddr_t _sbrk(int incr)
{
   char *prev_heap_start;
   char *next_heap_start;

   if (heap_start == 0) {
      _set_heap(fixup_end(), __heap_limit);
   }

   next_heap_start = heap_start + incr;

   if (check_stack(next_heap_start) && check_limit(next_heap_start)) {
      prev_heap_start = heap_start;
      heap_start = next_heap_start;

      return (caddr_t)(prev_heap_start);
   }
   else {
      errno = ENOMEM;
      return (caddr_t)(-1);
   }
}



