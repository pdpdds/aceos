include ../makefile.conf
MAKE:$(patsubst %.c,%.o,$(wildcard *.c))
	@echo "sys completed"

cputs.o: cputs.c ../include/stdio.h ../include/sys/version.h \
  ../include/sys/types.h
cputws.o: cputws.c ../include/string.h ../include/sys/types.h \
  ../include/stdlib.h
float_dx.o: float_dx.c ../include/libc/ieee.h
fmode.o: fmode.c
getenv.o: getenv.c
huge_val.o: huge_val.c ../include/libc/ieee.h
int64.o: int64.c ../include/sys/types.h
main.o: main.c ../include/stdio.h ../include/sys/version.h \
  ../include/sys/types.h ../include/string.h ../include/stdlib.h
morecore.o: morecore.c
#towc.o: towc.c ../include/stdlib.h ../include/sys/types.h ../include/stdio.h ../include/sys/version.h ../include/string.h ../include/libc/local.h
#ucfile.o: ucfile.c ../include/sys/types.h
#unicode.o: unicode.c ../include/wchar.h ../include/sys/types.h  ../include/stdlib.h
#wdprintf.o: wdprintf.c ../include/wchar.h ../include/sys/types.h ../include/stdarg.h ../include/stdio.h ../include/sys/version.h
write.o: write.c ../include/errno.h
