#
# Makefile for MPX
#

KERNEL_OBJECTS =\
kernel/irq.o\
kernel/core.o\
kernel/kmain.o\
kernel/serial.o\
kernel/pcb.o\
kernel/call.o\

LIB_OBJECTS =\
lib/ctype.o\
lib/stdlib.o\
lib/string.o\
lib/heap.o\

USER_OBJECTS =\
user/system.o\
user/commhand.o\
user/version.o\
user/time.o\
user/date.o\
user/shutdown.o\
user/help.o\
user/pcb_user.o\
user/loadr3.o\
user/alarm.o\
user/memory_user.o\

########################################################################
### Nothing below here needs to be changed
########################################################################

AS	= nasm
ASFLAGS = -f elf -g

CC	= clang
CFLAGS  = -std=c18 --target=i386-elf -Wall -Wextra -Werror -ffreestanding -g -Iinclude

ifeq ($(shell uname), Darwin)
LD	= i686-elf-ld
else
LD      = i686-linux-gnu-ld
endif
LDFLAGS = -melf_i386 -znoexecstack

OBJFILES = kernel/boot.o $(KERNEL_OBJECTS) $(LIB_OBJECTS) $(USER_OBJECTS)

all: kernel.bin

kernel.bin: $(OBJFILES) kernel/link.ld
	$(LD) $(LDFLAGS) -T kernel/link.ld -o $@ $(OBJFILES)

doc: Doxyfile
	doxygen

clean:
	rm -f $(OBJFILES) kernel.bin
