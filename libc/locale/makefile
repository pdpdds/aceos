include ../makefile.conf
MAKE:$(patsubst %.c,%.o,$(wildcard *.c))
	@echo "locale completed"
	
lconv.o: lconv.c ../include/locale.h ../include/limits.h
mbcurmax.o: mbcurmax.c ../include/stdlib.h ../include/sys/types.h
mblen.o: mblen.c ../include/stdlib.h ../include/sys/types.h
mbstowcs.o: mbstowcs.c ../include/stdlib.h ../include/sys/types.h
mbtowc.o: mbtowc.c ../include/stdlib.h ../include/sys/types.h
setlocal.o: setlocal.c ../include/locale.h ../include/string.h \
  ../include/sys/types.h
wcstombs.o: wcstombs.c ../include/stdlib.h ../include/sys/types.h
wctomb.o: wctomb.c ../include/stdlib.h ../include/sys/types.h
