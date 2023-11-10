#include "internal.h"

#include <xframe.h>

#include <sys/agb-hooks.h>

#define TAB_WIDTH 8



static int write_screen(int, char *, int);



static volatile unsigned short *regs;
static volatile unsigned short *vidmem;

static char *screen;
static int   width, height;
static int   cursor;

static int clear(int start, int end);
static int eol(int start);

static int x_scroll;
static int y_scroll;

static file_hook_t out = { 0, write_screen, 0, 0, 0 };
static file_hook_t err = { 0, write_screen, 0, 0, 0 };

static file_hook_t *files[3] = { 0, &out, &err };



void ex_start_tty (char *screen_in, const unsigned char *fontdata, int width_in, int height_in)
{
   unsigned short us;
   unsigned short r;
   unsigned short x;

   regs = (unsigned short *)0x4000000;

   regs[0x000/2] = 0x0101;
   regs[0x008/2] = 0x0D80;

   vidmem = (unsigned short *)0x5000000;
   vidmem[0] = 0x7C00;
   vidmem[1] = 0x7FFF;

   vidmem = (unsigned short *)0x6000000;

   for(x=0, r=0; r < 8192; r++)
   {
      us = fontdata[x+1];
      us <<= 8;
      us |= fontdata[x];
      x += 2;

      vidmem[r]=us;
   }

   vidmem = (unsigned short *)0x6006800;

   screen   = screen_in;
   width    = width_in;
   height   = height_in;

   x_scroll = 0;
   y_scroll = 0;

   set_file_hook_buffer(files, 3);
}



void ex_redraw_tty (void)
{
   int x, y;

   if (x_scroll < 0) {
      x_scroll = 0;
   }
   else if (x_scroll > width - 31) {
      x_scroll = width - 31;
   }

   if (y_scroll < 0) {
      y_scroll = y_scroll + height;
   }
   else if (y_scroll >= height) {
      y_scroll = y_scroll - height;
   }

   for (y = 0; y < 20; y++) {
      for (x = 0;x < 30; x++) {
         vidmem[y*32 + x] = screen[(x_scroll + y_scroll*width + y*width + x) % (width * height)];
      }
   }
}



int ex_poll_tty(void)
{
   #define KEY(x) (1<<x)

   short key = ~(*((volatile short*)0x04000130));

   if (key & KEY(4)) {
      x_scroll += 1;
   }

   if (key & KEY(5)) {
      x_scroll -= 1;
   }

   if (key & KEY(6)) {
      if (y_scroll > (cursor/width)) {
         y_scroll -= 1;

         if (y_scroll <= (cursor/width)) y_scroll = (cursor/width)+1;
      }
      else {
         y_scroll -= 1;
      }
   }

   if (key & KEY(7)) {
      if (y_scroll > (cursor/width)) {
         y_scroll += 1;
      }
      else {
         y_scroll += 1;

         if (y_scroll > ((cursor/width)-20)) y_scroll = ((cursor/width)-20);
      }
   }

   ex_redraw_tty();

   // start
   if (key & KEY(3)) {
      return 0;
   }

   return 1;

   #undef KEY
}



static int write_screen (__attribute__ ((unused)) int file, char *ptr, int len)
{
   int rv = len;
   int prev;
   int sol;

   while (len > 0) {
      switch (*ptr) {
         case '\n':
            cursor = eol(cursor);
            break;

         case '\r':
            cursor = cursor - (cursor % width);
            break;

         case '\t':
            prev = cursor;
            cursor = (cursor + TAB_WIDTH) - (cursor % TAB_WIDTH);

            sol = ((cursor / width) * width);

            if (cursor - sol < TAB_WIDTH) {
               cursor = sol;
            }

            clear(prev, cursor);
            break;

         case '\v':
            prev = cursor;
            cursor = (cursor + (width*20)) - (cursor % (width*20));
            clear(prev, cursor);
            break;

         default:
            screen[cursor++ % (width * height)] = *ptr;
            break;
      }

      ptr++;
      len--;
   }

   y_scroll = (cursor / width) - 20;

   x_scroll = (cursor % width) - 30;

   cursor %= (width * height);

   ex_redraw_tty();

   return rv;
}



static int clear(int start, int end)
{
   while (start < end) {
      screen[(start++) % (width * height)] = ' ';
   }

   return end;
}



static int eol(int start)
{
   int end = (start + width) - (start % width);
   return clear(start, end);
}



