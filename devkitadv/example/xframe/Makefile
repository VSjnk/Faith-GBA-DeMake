PREFIX=$(DEVKITADV)

CFLAGS=-Werror -Wall -W $(MACHINE) -Iinclude -O2 -g -mtune=arm7tdmi -mcpu=arm7tdmi
SFLAGS=-Werror -Wall -W $(MACHINE) -mcpu=arm7tdmi

CC=arm-agb-elf-gcc
AS=arm-agb-elf-as
AR=arm-agb-elf-ar
INSTALL=cp

TOOLDIR=$(PREFIX)/arm-agb-elf

XFRAME_FILES=$(MLIB)font-fixedsys.o $(MLIB)tty.o

.PHONY: all mlib-xframe xframe install mlib-install-xframe install-xframe multilib

all: mlib-xframe

mlib-xframe:
	make multilib TARGET=xframe

xframe: $(MLIB)/libxframe.a

$(MLIB)/libxframe.a: $(XFRAME_FILES)
	$(AR) r $(MLIB)/libxframe.a $(XFRAME_FILES)



install: install-headers mlib-install-xframe

install-headers:
	mkdir -p $(TOOLDIR)/include
	$(INSTALL) include/xframe.h $(TOOLDIR)/include

mlib-install-xframe:
	make multilib TARGET=install-xframe

install-xframe: $(MLIB)/libxframe.a
	mkdir -p $(TOOLDIR)/$(MLIB)
	$(INSTALL) $(MLIB)/libxframe.a $(TOOLDIR)/$(MLIB)



multilib:
	mkdir -p lib
	make $(TARGET) MACHINE="" MLIB="lib/"
	mkdir -p lib/interwork
	make $(TARGET) MACHINE="-mthumb-interwork" MLIB="lib/interwork/"
	mkdir -p lib/thumb/
	make $(TARGET) MACHINE="-mthumb"  MLIB="lib/thumb/"
	mkdir -p lib/thumb/interwork/
	make $(TARGET) MACHINE="-mthumb -mthumb-interwork" MLIB="lib/thumb/interwork/"



$(MLIB)%.o : %.S
	$(CC) $(MACHINE) $(SFLAGS) -o $@ -c $<

$(MLIB)%.o : %.c
	$(CC) $(MACHINE) $(CFLAGS) -o $@ -c $<



.PHONY: clean distclean

clean:
	rm -rf lib

distclean: clean

