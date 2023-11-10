CC=arm-agb-elf-gcc
OBJCOPY=arm-agb-elf-objcopy
CFLAGS=-mlong-calls -W -Wall -Werror
LFLAGS=-W -Wall -Werror -T$(DEVKITADV)/crtls/lnkscript
LFLAGS_AGBMAIN=$(LFLAGS) -nostartfiles
LIBS=-lxframe

.PHONY: all machine memfile

all:
	make machine TARGET="test-bin" NAME="AgbMain" MB="" OPTEXT=""
	make machine TARGET="test-mb"  NAME="AgbMain" MB="-DMB" OPTEXT=".mb"
	make heap TARGET="test-bin" CRT="$(CRT0)" NAME="AgbMain" MB="" OPTEXT=""
	make heap TARGET="test-mb"  CRT="$(CRT0)" NAME="AgbMain" MB="-DMB" OPTEXT=".mb"
	make test-bin NAME="AgbMain" CRT="$(CRT0)" STACK="" OPTEXT=""
	make test-bin NAME="AgbMain" CRT="$(CRT0)" STACK="-DSP_IRQ" OPTEXT=".irq"
	make test-bin NAME="AgbMain" CRT="$(CRT0)" STACK="-DSP_IRQ -DSP_USR" OPTEXT=".irq.usr"
	make test-bin NAME="AgbMain" CRT="$(CRT0)" STACK="-DSP_USR" OPTEXT=".usr"
	make test-bin NAME="AgbMain" CRT="$(CRT0)" INTR="-DINTR" OPTEXT=".intr"

heap:
	make data HEAP="" OPTEXT="$(OPTEXT)"
	make data HEAP="-DIHEAP" OPTEXT="$(OPTEXT).iheap"

data:
	make memfile DATA="" OPTEXT="$(OPTEXT)"
	make memfile DATA="-DEDATA" OPTEXT="$(OPTEXT).edata"

machine:
	make memfile CRT="$(CRT0)" MACHINE="" OPTEXT="$(OPTEXT)"
	make memfile CRT="$(CRT0_THUMB)" MACHINE="-mthumb" OPTEXT="$(OPTEXT).t"
	make memfile CRT="$(CRT0_THUMB_INTERWORK)" MACHINE="-mthumb -mthumb-interwork" OPTEXT="$(OPTEXT).t.i"
	make memfile CRT="$(CRT0_INTERWORK)" MACHINE="-mthumb-interwork" OPTEXT="$(OPTEXT).i"

memfile:
	make $(TARGET) MEMFILE="" OPTEXT="$(OPTEXT)"
	make $(TARGET) MEMFILE="-DMEMFILE_IWRAM" OPTEXT="$(OPTEXT).iwram"
	make $(TARGET) MEMFILE="-DMEMFILE_EWRAM" OPTEXT="$(OPTEXT).ewram"
	make $(TARGET) MEMFILE="-DMEMFILE_CARTROM" OPTEXT="$(OPTEXT).cartrom"

.PHONY: test-o test-elf test-bin

test-o:
	$(CC) $(MEMFILE) $(INTR) $(MB) $(STACK) $(HEAP) $(DATA) $(MACHINE) $(CFLAGS) -o $(NAME)$(OPTEXT).o -c $(NAME).c

test-elf: test-o
	$(CC) $(MACHINE) $(LFLAGS_AGBMAIN) -o $(NAME)$(OPTEXT).elf $(CRT) $(NAME)$(OPTEXT).o $(LIBS)

test-bin: test-elf
	$(OBJCOPY) -O binary $(NAME)$(OPTEXT).elf $(NAME)$(OPTEXT).bin

test-mb: test-bin
	cp $(NAME)$(OPTEXT).bin $(NAME)$(OPTEXT).mb

clean:
	rm -f *.elf *.bin *.mb *.o


