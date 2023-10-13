@echo off
echo Setting DevKit Advance CRT file environment variables...
echo DevKit Advance Install Directory: %DEVKITADV%

set _GCC_LIBDIR=%DEVKITADV%\lib\gcc-lib\arm-agb-elf\3.2.2
set _TOOL_LIBDIR=%DEVKITADV%\arm-agb-elf\lib

set CRTBEGIN=%_GCC_LIBDIR%\crtbegin.o
set CRTEND=%_GCC_LIBDIR%\crtend.o
set CRT0=%_TOOL_LIBDIR%\crt0.o

set CRTBEGIN_THUMB=%_GCC_LIBDIR%\thumb\crtbegin.o
set CRTEND_THUMB=%_GCC_LIBDIR%\thumb\crtend.o
set CRT0_THUMB=%_TOOL_LIBDIR%\thumb\crt0.o

set CRTBEGIN_INTERWORK=%_GCC_LIBDIR%\interwork\crtbegin.o
set CRTEND_INTERWORK=%_GCC_LIBDIR%\interwork\crtend.o
set CRT0_INTERWORK=%_TOOL_LIBDIR%\interwork\crt0.o

set CRTBEGIN_THUMB_INTERWORK=%_GCC_LIBDIR%\thumb\interwork\crtbegin.o
set CRTEND_THUMB_INTERWORK=%_GCC_LIBDIR%\thumb\interwork\crtend.o
set CRT0_THUMB_INTERWORK=%_TOOL_LIBDIR%\thumb\interwork\crt0.o

set _GCC_LIBDIR=
set _TOOL_LIBDIR=
