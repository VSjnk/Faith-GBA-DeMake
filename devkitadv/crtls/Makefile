PREFIX=$(DEVKITADV)

CFLAGS=-Werror -Wall -W $(MACHINE) -O2 -g -mtune=arm7tdmi -mcpu=arm7tdmi
SFLAGS=-Werror -Wall -W $(MACHINE) -mcpu=arm7tdmi
LFLAGS=-Werror -Wall -W $(MACHINE) -Tlnkscript

CC=arm-agb-elf-gcc
CXX=arm-agb-elf-g++
AS=arm-agb-elf-as
LD=arm-agb-elf-ld
NM=arm-agb-elf-nm
OBJDUMP=arm-agb-elf-objdump
OBJCOPY=arm-agb-elf-objcopy

TOOLDIR=$(PREFIX)/arm-agb-elf

LIBC_FILES=$(MLIB)access.o $(MLIB)agb-alarm.o $(MLIB)agb-abort.o $(MLIB)setjmp.o $(MLIB)agbmain.o $(MLIB)agb-close.o $(MLIB)agb-exit.o $(MLIB)agb-file-hooks.o $(MLIB)agb-filesystem-hooks.o $(MLIB)agb-fstat.o $(MLIB)agb-getpid.o $(MLIB)agb-gettimeofday.o $(MLIB)agb-isatty.o $(MLIB)agb-kill.o $(MLIB)agb-link.o $(MLIB)agb-lseek.o $(MLIB)agb-open.o $(MLIB)agb-raise.o $(MLIB)agb-read.o $(MLIB)agb-rename.o $(MLIB)agb-sbrk.o $(MLIB)agb-stat.o $(MLIB)agb-system.o $(MLIB)agb-times.o $(MLIB)agb-unlink.o $(MLIB)agb-write.o
CRT0_FILES=$(MLIB)crt0.o

.PHONY: all libc crt0 ilibc icrt0 install-libc install-crt0

all:
	make multilib TARGET=libc
	make multilib TARGET=crt0

libc: $(LIBC_FILES)

ilibc: libc
	ar r $(TOOLDIR)/$(MLIB)libc.a $(LIBC_FILES)

crt0: $(CRT0_FILES)

icrt0: crt0
	cp $(MLIB)crt0.o $(TOOLDIR)/$(MLIB)crt0.o

multilib:
	mkdir -p lib
	make $(TARGET) MACHINE="" MLIB="lib/"
	mkdir -p lib/interwork
	make $(TARGET) MACHINE="-mthumb-interwork" MLIB="lib/interwork/"
	mkdir -p lib/thumb/
	make $(TARGET) MACHINE="-mthumb"  MLIB="lib/thumb/"
	mkdir -p lib/thumb/interwork/
	make $(TARGET) MACHINE="-mthumb -mthumb-interwork" MLIB="lib/thumb/interwork/"

install-crt0:
	make multilib TARGET=icrt0

install-libc:
	make multilib TARGET=ilibc

install-headers:
	mkdir -p $(TOOLDIR)/include/sys
	cp agb-hooks.h $(TOOLDIR)/include/sys
	cp asm-macros.h $(TOOLDIR)/include/sys

install: install-crt0 install-libc install-headers

$(MLIB)%.o : %.S
	$(CC) $(MACHINE) $(SFLAGS) -o $@ -c $<

$(MLIB)%.o : %.c
	$(CC) $(MACHINE) $(CFLAGS) -o $@ -c $<

.PHONY: clean

clean:
	rm -rf lib



