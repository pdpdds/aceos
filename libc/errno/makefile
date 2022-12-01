include ../makefile.conf
MAKE:$(patsubst %.c,%.o,$(wildcard *.c))
	@echo "errno completed"
errno.o: errno.c ../include/errno.h ../include/sys/types.h