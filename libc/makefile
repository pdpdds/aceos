#                   Std C Lib for Ace OS - makefile for gcc 3.1 
#				- Created by Sam (samueldotj@gmail.com) 
#------------------------------------------------------------------------------------------------
ASM_COMPILER = nasmw
GCC_COMPILER = gcc
GXX_COMPILER = g++
AR = ar
INCLUDE_DIR = include

ASM_OPTIONS = -i $(INCLUDE_DIR) -w+orphan-labels -f win32
GCC_OPTIONS = -m32 -nostartfiles -ffreestanding -c -I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/OS
GXX_OPTIONS = -m32 -nostartfiles -fno-exceptions -ffreestanding -c  -I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/OS

ALL = assert crt0 ctype errno math stdio stdlib string sys time locale ace setjmp \
	compat/bsd compat/io  compat/ioctl compat/math compat/mman compat/mntent compat/search compat/signal \
	compat/stdio compat/stdlib compat/string compat/sys compat/termios compat/time compat/unistd compat/v1 \
	posix/dirent posix/fcntl posix/fnmatch posix/glob posix/grp posix/pwd \
	posix/regex posix/setjmp posix/signal posix/stdio posix/sys posix/sys/stat posix/sys/times posix/sys/wait \
	posix/termios posix/unistd posix/utime posix/utsname

.PHONY: ALL Clean

CLIB	:	$(ALL)
	rm -rf libc.a
	$(AR) q libc.a $(wildcard assert/*.o) $(wildcard crt0/*.o) $(wildcard ctype/*.o) $(wildcard errno/*.o) $(wildcard math/*.o) $(wildcard stdio/*.o) \
			$(wildcard stdlib/*.o) $(wildcard string/*.o) $(wildcard sys/*.o) $(wildcard time/*.o) $(wildcard locale/*.o) $(wildcard setjmp/*.o) $(wildcard ace/*.o) \
			$(wildcard compat/bsd/*.o) $(wildcard compat/io/*.o)  $(wildcard compat/ioctl/*.o) $(wildcard compat/math/*.o) $(wildcard compat/mman/*.o) $(wildcard compat/mntent/*.o) $(wildcard compat/search/*.o) $(wildcard compat/signal/*.o) \
			$(wildcard compat/stdio/*.o) $(wildcard compat/stdlib/*.o) $(wildcard compat/string/*.o) $(wildcard compat/sys/*.o) $(wildcard compat/termios/*.o) $(wildcard compat/time/*.o) $(wildcard compat/unistd/*.o) $(wildcard compat/v1/*.o) \
			$(wildcard posix/dirent/*.o) $(wildcard posix/fcntl/*.o) $(wildcard posix/fnmatch/*.o) $(wildcard posix/glob/*.o) $(wildcard posix/grp/*.o) \
			$(wildcard posix/pwd/*.o) $(wildcard posix/regex/*.o) $(wildcard posix/setjmp/*.o) $(wildcard posix/signal/*.o) $(wildcard posix/stdio/*.o) \
			$(wildcard posix/sys/stat/*.o) $(wildcard posix/sys/times/*.o) $(wildcard posix/sys/wait/*.o) \
			$(wildcard posix/termios/*.o) $(wildcard posix/unistd/*.o) $(wildcard posix/utime/*.o) $(wildcard posix/utsname/*.o)
	ld $(wildcard assert/*.o) $(wildcard crt0/*.o) $(wildcard ctype/*.o) $(wildcard errno/*.o) $(wildcard math/*.o) $(wildcard stdio/*.o) \
			$(wildcard stdlib/*.o) $(wildcard string/*.o) $(wildcard sys/*.o) $(wildcard time/*.o) $(wildcard locale/*.o) $(wildcard setjmp/*.o)  $(wildcard ace/*.o) \
			$(wildcard compat/bsd/*.o) $(wildcard compat/io/*.o)  $(wildcard compat/ioctl/*.o) $(wildcard compat/math/*.o) $(wildcard compat/mman/*.o) $(wildcard compat/mntent/*.o) $(wildcard compat/search/*.o) $(wildcard compat/signal/*.o) \
			$(wildcard compat/stdio/*.o) $(wildcard compat/stdlib/*.o) $(wildcard compat/string/*.o) $(wildcard compat/sys/*.o) $(wildcard compat/termios/*.o) $(wildcard compat/time/*.o) $(wildcard compat/unistd/*.o) $(wildcard compat/v1/*.o) \
			$(wildcard posix/dirent/*.o) $(wildcard posix/fcntl/*.o) $(wildcard posix/fnmatch/*.o) $(wildcard posix/glob/*.o) $(wildcard posix/grp/*.o) \
			$(wildcard posix/pwd/*.o) $(wildcard posix/regex/*.o) $(wildcard posix/setjmp/*.o) $(wildcard posix/signal/*.o) $(wildcard posix/stdio/*.o) \
			$(wildcard posix/sys/stat/*.o) $(wildcard posix/sys/times/*.o) $(wildcard posix/sys/wait/*.o) \
			$(wildcard posix/termios/*.o) $(wildcard posix/unistd/*.o) $(wildcard posix/utime/*.o) $(wildcard posix/utsname/*.o) \
			-L ..\Kernel32.dll -l Kernel32
			
	#$(MAKE) -C test
clean:
	rm -rf *.o /s
	rm -rf *.a /s

assert	:	ALWAYSRUN
	$(MAKE) -C assert
crt0	:	ALWAYSRUN
	$(MAKE) -C crt0
ctype	:	ALWAYSRUN
	$(MAKE) -C ctype
errno	:	ALWAYSRUN
	$(MAKE) -C errno
math	:	ALWAYSRUN
	$(MAKE) -C math
stdio	:	ALWAYSRUN
	$(MAKE) -C stdio
stdlib	:	ALWAYSRUN
	$(MAKE) -C stdlib
string	:	ALWAYSRUN
	$(MAKE) -C string
sys	:	ALWAYSRUN
	$(MAKE) -C sys
time	:	ALWAYSRUN
	$(MAKE) -C time
locale	:	ALWAYSRUN
	$(MAKE) -C locale
setjmp 	:	ALWAYSRUN
	$(MAKE) -C setjmp
ace 	:	ALWAYSRUN
	$(MAKE) -C ace

compat/bsd :	ALWAYSRUN
	$(MAKE) -C compat/bsd
compat/io :	ALWAYSRUN
	$(MAKE) -C compat/io
compat/ioctl :	ALWAYSRUN
	$(MAKE) -C compat/ioctl
compat/math :	ALWAYSRUN
	$(MAKE) -C compat/math
compat/mman :	ALWAYSRUN
	$(MAKE) -C compat/mman
compat/mntent :	ALWAYSRUN
	$(MAKE) -C compat/mntent
compat/search :	ALWAYSRUN
	$(MAKE) -C compat/search
compat/signal :	ALWAYSRUN
	$(MAKE) -C compat/signal
compat/stdio :	ALWAYSRUN
	$(MAKE) -C compat/stdio
compat/stdlib :	ALWAYSRUN
	$(MAKE) -C compat/stdlib
compat/string :	ALWAYSRUN
	$(MAKE) -C compat/string
compat/sys :	ALWAYSRUN
	$(MAKE) -C compat/sys
compat/termios : ALWAYSRUN
	$(MAKE) -C compat/termios
compat/tim :	ALWAYSRUN
	$(MAKE) -C compat/time
compat/bsd :	ALWAYSRUN
	$(MAKE) -C compat/bsd
compat/unistd :	ALWAYSRUN
	$(MAKE) -C compat/unistd
compat/v1 :	ALWAYSRUN
	$(MAKE) -C compat/v1











posix/dirent : ALWAYSRUN
	$(MAKE) -C posix/dirent
posix/fcntl : ALWAYSRUN
	$(MAKE) -C posix/fcntl
posix/fnmatch : ALWAYSRUN
	$(MAKE) -C posix/fnmatch
posix/glob : ALWAYSRUN
	$(MAKE) -C posix/glob
posix/grp : ALWAYSRUN
	$(MAKE) -C posix/grp
posix/pwd : ALWAYSRUN
	$(MAKE) -C posix/pwd
posix/regex : ALWAYSRUN
	$(MAKE) -C posix/regex
posix/setjmp : ALWAYSRUN
	$(MAKE) -C posix/setjmp
posix/signal : ALWAYSRUN
	$(MAKE) -C posix/signal
posix/stdio : ALWAYSRUN
	$(MAKE) -C posix/stdio
posix/sys : ALWAYSRUN
	$(MAKE) -C posix/sys
posix/sys/stat : ALWAYSRUN
	$(MAKE) -C posix/sys/stat
posix/sys/times : ALWAYSRUN
	$(MAKE) -C posix/sys/times
posix/sys/wait : ALWAYSRUN
	$(MAKE) -C posix/sys/wait
posix/termios : ALWAYSRUN
	$(MAKE) -C posix/termios
posix/unistd : ALWAYSRUN
	$(MAKE) -C posix/unistd
posix/utime : ALWAYSRUN
	$(MAKE) -C posix/utime
posix/utsname : ALWAYSRUN
	$(MAKE) -C posix/utsname
posix/sys/stat : ALWAYSRUN
	$(MAKE) -C posix/sys/stat
ALWAYSRUN:
