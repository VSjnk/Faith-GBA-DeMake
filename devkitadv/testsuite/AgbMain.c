#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <xframe.h>

#define EWRAM 1
#define IWRAM 2
#define CARTROM 3
#define INVALID 0

#ifdef INTR
void __interrupt_handler(void)
{
}
#endif

#ifdef MEMFILE_CARTROM

#ifdef MB
#define VAR_LOCATION EWRAM
#define VAR_WRITABLE 1
#define ROVAR_LOCATION EWRAM
#define ROVAR_WRITABLE 1
#else
#define VAR_LOCATION CARTROM
#define VAR_WRITABLE 0
#define ROVAR_LOCATION CARTROM
#define ROVAR_WRITABLE 0
#endif

#endif

#ifdef MEMFILE_IWRAM
#define VAR_LOCATION IWRAM
#define VAR_WRITABLE 1
#define ROVAR_LOCATION IWRAM
#define ROVAR_WRITABLE 1
#endif

#ifdef MEMFILE_EWRAM
#define VAR_LOCATION EWRAM
#define VAR_WRITABLE 1
#define ROVAR_LOCATION EWRAM
#define ROVAR_WRITABLE 1
#endif

#ifdef MB
__attribute__ ((section(".devkitadv.config"))) int __gba_multiboot;
#define CARTROM_LOCATION EWRAM
#define CARTROM_WRITABLE 1
#define APPENDED_LOCATION EWRAM
#define APPENDED_WRITABLE 1

#ifndef ROVAR_LOCATION
#define ROVAR_LOCATION EWRAM
#endif

#ifndef ROVAR_WRITABLE
#define ROVAR_WRITABLE 1
#endif

#else

#define CARTROM_LOCATION CARTROM
#define CARTROM_WRITABLE 0
#define APPENDED_LOCATION CARTROM
#define APPENDED_WRITABLE 0

#ifndef ROVAR_LOCATION
#define ROVAR_LOCATION CARTROM
#endif

#ifndef ROVAR_WRITABLE
#define ROVAR_WRITABLE 0
#endif

#endif

#ifdef SP_USR
__attribute__ ((section(".cartrom.data"))) int *const __sp_usr_initial_value = (int *)(0x02040000 - 0x100);
#define SP_USR_LOCATION EWRAM
#else
#define SP_USR_LOCATION IWRAM
#endif

#ifdef SP_IRQ
__attribute__ ((section(".cartrom.data"))) int *const __sp_irq_initial_value = (int *)(0x02040000 - 0x060);
#define SP_IRQ_LOCATION EWRAM
#else
#define SP_IRQ_LOCATION IWRAM
#endif

#ifdef IHEAP
__attribute__ ((section(".devkitadv.config"))) int __gba_iwram_heap;
#define MALLOC_LOCATION IWRAM
#else
#define MALLOC_LOCATION EWRAM
#endif

#ifdef EDATA
__attribute__ ((section(".devkitadv.config"))) int __gba_ewram_data;
#define BSS_LOCATION EWRAM

#ifndef VAR_LOCATION
#define VAR_LOCATION EWRAM
#endif

#ifndef VAR_WRITABLE
#define VAR_WRITABLE 1
#endif

#else
#define BSS_LOCATION IWRAM

#ifndef VAR_LOCATION
#define VAR_LOCATION IWRAM
#endif

#ifndef VAR_WRITABLE
#define VAR_WRITABLE 1
#endif

#endif

extern char __ewram_overlay_start[];
extern char __load_start_ewram0[];
extern char __load_start_ewram1[];
extern char __load_start_ewram2[];
extern char __load_start_ewram3[];
extern char __load_start_ewram4[];
extern char __load_start_ewram5[];
extern char __load_start_ewram6[];
extern char __load_start_ewram7[];
extern char __load_start_ewram8[];
extern char __load_start_ewram9[];
extern char __load_stop_ewram0[];
extern char __load_stop_ewram1[];
extern char __load_stop_ewram2[];
extern char __load_stop_ewram3[];
extern char __load_stop_ewram4[];
extern char __load_stop_ewram5[];
extern char __load_stop_ewram6[];
extern char __load_stop_ewram7[];
extern char __load_stop_ewram8[];
extern char __load_stop_ewram9[];

extern char __iwram_overlay_start[];
extern char __load_start_iwram0[];
extern char __load_start_iwram1[];
extern char __load_start_iwram2[];
extern char __load_start_iwram3[];
extern char __load_start_iwram4[];
extern char __load_start_iwram5[];
extern char __load_start_iwram6[];
extern char __load_start_iwram7[];
extern char __load_start_iwram8[];
extern char __load_start_iwram9[];
extern char __load_stop_iwram0[];
extern char __load_stop_iwram1[];
extern char __load_stop_iwram2[];
extern char __load_stop_iwram3[];
extern char __load_stop_iwram4[];
extern char __load_stop_iwram5[];
extern char __load_stop_iwram6[];
extern char __load_stop_iwram7[];
extern char __load_stop_iwram8[];
extern char __load_stop_iwram9[];

#define SECTION(s, x, t) __attribute__ ((section(s))) int x[5] = { t + 1, t + 2, t + 3, t + 4, t + 5 }
#define OVERLAY(s, x, t, n) __attribute__ ((section(s))) int x[n] = { t + 1, t + 2, t + 3, t + 4, t + 5 }
#define ARRAY(x, t) int x[5] = { t + 1, t + 2, t + 3, t + 4, t + 5 }

#define PALRAM ((volatile unsigned short *)0x05000000)

#define   TEST(t) printf("%s%s\n", ((t)?("  "):(PALRAM[0] = RGB(31,0,0),   errors++, "!-")), #t)

#ifdef MB
#define RETEST(t) printf("%s%s\n", ((t)?(resuccesses++, "@-"):(reerrors++, "*-")), #t)
#else
#define RETEST(t) TEST(t)
#endif

#define OVERLAY_TEST(d, x, r, t) \
   memcpy(d, __load_start_##x, (char*)&__load_stop_##x - (char*)&__load_start_##x); \
   TEST(location(x) == r);	\
   TEST(read_test(x, t));	\
   TEST(write_test(x, t * 10));


SECTION(".ewram.data", ewram, 100);
SECTION(".ewram.data", notouch_ewram, 150);

#ifdef MB
#define EWRAM_OVERLAY(s, x, t, n)  __attribute__ ((section(".ewram.data"))) int x[n] = { t + 1, t + 2, t + 3, t + 4, t + 5 }
#define EWRAM_OVERLAY_TEST(d, x, r, t) \
     TEST(location(x) == r);	\
   RETEST(read_test(x, t));	\
     TEST(write_test(x, t * 10));
#else
#define EWRAM_OVERLAY(s, x, t, n) OVERLAY(s, x, t, n)
#define EWRAM_OVERLAY_TEST(d, x, r, t) OVERLAY_TEST(d, x, r, t)
#endif

EWRAM_OVERLAY(".ewram0.data", ewram0, 200, 9);
EWRAM_OVERLAY(".ewram1.data", ewram1, 210, 5);
EWRAM_OVERLAY(".ewram2.data", ewram2, 220, 11);
//EWRAM_OVERLAY(".ewram3.data", ewram3, 130, 7);
EWRAM_OVERLAY(".ewram4.data", ewram4, 240, 13);
EWRAM_OVERLAY(".ewram5.data", ewram5, 250, 9);
EWRAM_OVERLAY(".ewram6.data", ewram6, 260, 15);
//EWRAM_OVERLAY(".ewram7.data", ewram7, 270, 11);
EWRAM_OVERLAY(".ewram8.data", ewram8, 280, 17);
EWRAM_OVERLAY(".ewram9.data", ewram9, 290, 13);

SECTION(".iwram.data", iwram, 300);
SECTION(".iwram.data", notouch_iwram, 350);

OVERLAY(".iwram0.data", iwram0, 400, 6);
OVERLAY(".iwram1.data", iwram1, 410, 10);
OVERLAY(".iwram2.data", iwram2, 420, 12);
OVERLAY(".iwram3.data", iwram3, 430, 8);
OVERLAY(".iwram4.data", iwram4, 440, 14);
//OVERLAY(".iwram5.data", iwram5, 450, 10);
OVERLAY(".iwram6.data", iwram6, 460, 16);
OVERLAY(".iwram7.data", iwram7, 470, 12);
OVERLAY(".iwram8.data", iwram8, 480, 18);
//OVERLAY(".iwram9.data", iwram9, 490, 14);

const SECTION(".cartrom.data", cartrom, 500);

ARRAY(init, 600);
ARRAY(notouch_init, 650);
const ARRAY(const_init, 700);
const ARRAY(notouch_const_init, 750);
int uninit[5];

SECTION(".appended.data", appended, 800);
SECTION(".appended.data", notouch_appended, 850);

int errors;
int reerrors;
int resuccesses;

__attribute__ ((always_inline, const)) static inline int RGB(int r, int g, int b)
{
   return r | g << 5 | b << 10;
}

__attribute__ ((section(".ewram.data"))) char screen[80 * 300];

int location(const void *ptr)
{
   if (ptr >= (void*)0x02000000 && ptr < (void*)0x02040000) {
      return EWRAM;
   }
   else if (ptr >= (void*)0x03000000 && ptr < (void*)0x03008000) {
      return IWRAM;
   }
   else if (ptr >= (void*)0x08000000 && ptr < (void*)0x0a000000) {
      return CARTROM;
   }
   else {
      return INVALID;
   }
}

int read_test(const int *array, int start)
{
   int i;

   for (i = 0; i < 5; i++) {
      start++;

      //printf("(%p) %d == %d\n", array + i, array[i], start);

      if (array[i] != start) {
         return 0;
      }
   }

   return 1;
}

int write_test(int *array, int start)
{
   int i;

   for (i = 0; i < 5; i++) {
      array[i] = ++start;

      if (array[i] != start) {
         return 0;
      }
   }

   return 1;
}

int is_zero(int *array)
{
   int i;

   for (i = 0; i < 5; i++) {
      if (array[i] != 0) {
         return 0;
      }
   }

   return 1;
}

extern int __crt0_mb_loaded;
typedef void (*fun)(void);



void AgbMain(void)
{
   int stack_var;
   int *ptr;

#if defined(MB) && defined(IHEAP) && defined(EDATA)
   extern void _set_heap(char*,char*);
   extern char __ewram_break[];
   extern char __memmap_ewram_end[];

   // work around reset-error in malloc
   _set_heap(__ewram_break, __memmap_ewram_end);
#endif

   memset(screen, ' ', 80 * 200);

   ex_start_tty(screen, ex_font_fixedsys, 80, 200);

     TEST(location(ewram) == EWRAM);
   RETEST(read_test(ewram, 100));
     TEST(write_test(ewram, 1000));

     TEST(location(ewram0) == EWRAM);
   RETEST(read_test(ewram0, 200));
     TEST(write_test(ewram0, 2000));

   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram1, EWRAM, 210);
   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram2, EWRAM, 220);
// EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram3, EWRAM, 230);
   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram4, EWRAM, 240);
   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram5, EWRAM, 250);
   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram6, EWRAM, 260);
// EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram7, EWRAM, 270);
   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram8, EWRAM, 280);
   EWRAM_OVERLAY_TEST(__ewram_overlay_start, ewram9, EWRAM, 290);

   TEST(location(iwram) == IWRAM);
   TEST(read_test(iwram, 300));
   TEST(write_test(iwram, 3000));

   TEST(location(iwram0) == IWRAM);
   TEST(read_test(iwram0, 400));
   TEST(write_test(iwram0, 4000));

   OVERLAY_TEST(__iwram_overlay_start, iwram1, IWRAM, 410);
   OVERLAY_TEST(__iwram_overlay_start, iwram2, IWRAM, 420);
   OVERLAY_TEST(__iwram_overlay_start, iwram3, IWRAM, 430);
   OVERLAY_TEST(__iwram_overlay_start, iwram4, IWRAM, 440);
// OVERLAY_TEST(__iwram_overlay_start, iwram5, IWRAM, 450);
   OVERLAY_TEST(__iwram_overlay_start, iwram6, IWRAM, 460);
   OVERLAY_TEST(__iwram_overlay_start, iwram7, IWRAM, 470);
   OVERLAY_TEST(__iwram_overlay_start, iwram8, IWRAM, 480);
// OVERLAY_TEST(__iwram_overlay_start, iwram9, IWRAM, 490);

     TEST(location(cartrom) == CARTROM_LOCATION);
   RETEST(read_test(cartrom, 500));
     TEST(write_test((int*)cartrom, 5000) == CARTROM_WRITABLE);

     TEST(location(init) == VAR_LOCATION);
#if (defined(EDATA) || defined(MEMFILE_CARTROM) || defined(MEMFILE_EWRAM)) && !defined(MEMFILE_IWRAM)
   RETEST(read_test(init, 600));
#else
     TEST(read_test(init, 600));
#endif
     TEST(write_test(init, 6000) == VAR_WRITABLE);

     TEST(location(const_init) == ROVAR_LOCATION);
#if defined(MEMFILE_IWRAM)
     TEST(read_test(const_init, 700));
#else
   RETEST(read_test(const_init, 700));
#endif
     TEST(write_test((int*)const_init, 7000) == ROVAR_WRITABLE);

     TEST(location(appended) == APPENDED_LOCATION);
   RETEST(read_test(appended, 800));
     TEST(write_test((int*)appended, 8000) == APPENDED_WRITABLE);

   TEST(location(uninit) == BSS_LOCATION);
   TEST(is_zero(uninit));
   TEST(write_test(uninit, 9000));

   TEST(read_test(ewram, 1000));
   TEST(read_test(iwram, 3000));
   TEST(CARTROM_WRITABLE ? read_test(cartrom, 5000) : read_test(cartrom, 500));
   TEST(read_test(init, 6000) == VAR_WRITABLE);
   TEST(ROVAR_WRITABLE ? read_test(const_init, 7000) : read_test(const_init, 700));
   TEST(APPENDED_WRITABLE ? read_test(appended, 8000) : read_test(appended, 800));
   TEST(read_test(uninit, 9000));

   TEST(location(&stack_var) == SP_USR_LOCATION);

   TEST((ptr = (int*)malloc(100)));

#if defined(MB) && defined(IHEAP) && defined(EDATA)
   // the heap was moved earlier to work around a conflict
   TEST(location(ptr) == EWRAM);
#else
   TEST(location(ptr) == MALLOC_LOCATION);
#endif
	
   TEST(location(notouch_ewram) == EWRAM);
   TEST(read_test(notouch_ewram, 150));

   TEST(location(notouch_iwram) == IWRAM);
   TEST(read_test(notouch_iwram, 350));

   TEST(location(notouch_init) == VAR_LOCATION);
   TEST(read_test(notouch_init, 650));

   TEST(location(notouch_const_init) == ROVAR_LOCATION);
   TEST(read_test(notouch_const_init, 750));

   TEST(location(notouch_appended) == APPENDED_LOCATION);
   TEST(read_test(notouch_appended, 850));

   printf("%d error%s.\n", errors, errors==1?"":"s");
   printf("%d reset-error%s.\n", reerrors, reerrors==1?"":"s");
   printf("%d reset-success%s.\n", resuccesses, resuccesses==1?"":"es");

   if (resuccesses > 0 && reerrors > 0) {
      PALRAM[0] = RGB(31,0,0);
      printf("!all RETESTs should fail or succeed together!\n");
   }

   printf("Press [START] to exit...\n");

   while (ex_poll_tty()) /**/ ;
}


//#define LCDMODE (*(volatile unsigned short *)0x04000000)
//#define PALRAM ((volatile unsigned short *)0x05000000)

//   int i;

//   LCDMODE = 0;

//   for (i = 0; i < 1000; i++) {
//      PALRAM[0] = RGB(31,31,0);
//   }
//   for (i = 0; i < 1000; i++) {
//      PALRAM[0] = RGB(0,31,31);
//   }


