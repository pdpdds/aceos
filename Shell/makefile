#                   Ace OS Shell makefile for mingw
#				- Created by Sam (samueldotj@gmail.com) 
#------------------------------------------------------------------------------------------------
INCLUDE_DIR = Include
CFLAGS =	-m32 -g -Wall -Wno-multichar  -nostartfiles -ffreestanding -c -I $(INCLUDE_DIR) -I ../$(INCLUDE_DIR)
CPPFLAGS =	-m32 -g -fno-rtti -fno-exceptions -nostartfiles -ffreestanding -c -I $(INCLUDE_DIR) -I ../$(INCLUDE_DIR)
CC=		gcc
LD=		ld
#------------------------------------------------------------------------------------------------
MAKE:$(patsubst %.c,%.o,$(wildcard *.c))
	$(CC) -g $(patsubst %.c,%.o,$(wildcard *.c)) -o Shell.exe

clean:
	rm -rf Shell.exe
	rm -rf *.o