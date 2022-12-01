AsmCompiler = nasm
gccCompiler = gcc
gxxCompiler = g++

IncludeDir = ../Include

AsmOptions = -i $(IncludeDir) -w+orphan-labels -f win32
gccOptions = -nostartfiles -ffreestanding -c -I $(IncludeDir) -m32 -fno-use-cxa-atexit 
gxxOptions = -nostartfiles -fno-exceptions -ffreestanding -c  -I $(IncludeDir) -m32 -fno-use-cxa-atexit 


OBJECTFILES = strcat.o strchr.o strcmp.o strcpy.o strcspn.o strdup.o stricmp.o \
		strlen.o strlwr.o strncat.o strncmp.o strncpy.o strnset.o strnicmp.o \
		strpbrk.o strrchr.o strrev.o strset.o strspn.o strstr.o strupr.o strnchr.o \
		memcpy.o memchr.o memcmp.o memmove.o memset.o memicmp.o  memccpy.o

TARGET:	$(OBJECTFILES)
	@ECHO "Build Sucess"
strcat.o	:	$(IncludeDir)/string.h strcat.c
	$(gccCompiler) $(gccOptions) strcat.c
strchr.o	:	$(IncludeDir)/string.h strchr.c
	$(gccCompiler) $(gccOptions) strchr.c
strcmp.o	:	$(IncludeDir)/string.h strcmp.c
	$(gccCompiler) $(gccOptions) strcmp.c
strcpy.o	:	$(IncludeDir)/string.h strcpy.c
	$(gccCompiler) $(gccOptions) strcpy.c
strcspn.o	:	$(IncludeDir)/string.h strcspn.c
	$(gccCompiler) $(gccOptions) strcspn.c
strdup.o	:	$(IncludeDir)/string.h strdup.c
	$(gccCompiler) $(gccOptions) strcat.c
stricmp.o	:	$(IncludeDir)/string.h stricmp.c
	$(gccCompiler) $(gccOptions) stricmp.c
strlen.o	:	$(IncludeDir)/string.h strlen.c
	$(gccCompiler) $(gccOptions) strlen.c
strlwr.o	:	$(IncludeDir)/string.h strlwr.c
	$(gccCompiler) $(gccOptions) strlwr.c
strncat.o	:	$(IncludeDir)/string.h strncat.c
	$(gccCompiler) $(gccOptions) strncat.c
strncmp.o	:	$(IncludeDir)/string.h strncmp.c
	$(gccCompiler) $(gccOptions) strncmp.c
strncpy.o	:	$(IncludeDir)/string.h strncpy.c
	$(gccCompiler) $(gccOptions) strncpy.c
strnset.o	:	$(IncludeDir)/string.h strnset.c
	$(gccCompiler) $(gccOptions) strnset.c
strpbrk.o	:	$(IncludeDir)/string.h strpbrk.c
	$(gccCompiler) $(gccOptions) strpbrk.c
strrchr.o	:	$(IncludeDir)/string.h strrchr.c
	$(gccCompiler) $(gccOptions) strrchr.c
strrev.o	:	$(IncludeDir)/string.h strrev.c
	$(gccCompiler) $(gccOptions) strrev.c
strset.o	:	$(IncludeDir)/string.h strset.c
	$(gccCompiler) $(gccOptions) strset.c
strspn.o	:	$(IncludeDir)/string.h strspn.c
	$(gccCompiler) $(gccOptions) strspn.c
strstr.o	:	$(IncludeDir)/string.h strstr.c
	$(gccCompiler) $(gccOptions) strstr.c
strupr.o	:	$(IncludeDir)/string.h strupr.c
	$(gccCompiler) $(gccOptions) strupr.c
strnicmp.o	:	$(IncludeDir)/string.h strnicmp.c
	$(gccCompiler) $(gccOptions) strnicmp.c
strnchr.o	:	$(IncludeDir)/string.h strnchr.c
	$(gccCompiler) $(gccOptions) strnchr.c
memcpy.o	:	$(IncludeDir)/string.h memcpy.c
	$(gccCompiler) $(gccOptions) memcpy.c
memchr.o	:	$(IncludeDir)/string.h memchr.c
	$(gccCompiler) $(gccOptions) memchr.c
memcmp.o	:	$(IncludeDir)/string.h memcmp.c
	$(gccCompiler) $(gccOptions) memcmp.c
memmove.o	:	$(IncludeDir)/string.h memmove.c
	$(gccCompiler) $(gccOptions) memmove.c
memset.o	:	$(IncludeDir)/string.h memset.c
	$(gccCompiler) $(gccOptions) memset.c
memicmp.o	:	$(IncludeDir)/string.h memicmp.c
	$(gccCompiler) $(gccOptions) memicmp.c
memccpy.o	:	$(IncludeDir)/string.h memccpy.c
	$(gccCompiler) $(gccOptions) memccpy.c













