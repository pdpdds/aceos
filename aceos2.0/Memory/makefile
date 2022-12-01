AsmCompiler = nasm
gccCompiler = gcc
gxxCompiler = g++

IncludeDir = ../Include

AsmOptions = -i $(IncludeDir) -w+orphan-labels -f win32
gccOptions = -D KERNELBUILT -nostartfiles -ffreestanding -fno-use-cxa-atexit -c -I $(IncludeDir) -Wall -m32
gxxOptions = -D KERNELBUILT -nostartfiles -fno-exceptions -fno-use-cxa-atexit -ffreestanding -c  -I $(IncludeDir) -Wall -m32


OBJECTFILES = Descriptor.o VASM.o Bitmap.o CPhysical.o Physical.o Heap.o malloc.o

TARGET:	$(OBJECTFILES)
	@ECHO "Build Sucess"
VASM.o	:	$(IncludeDir)/Memory.h VASM.c
	$(gccCompiler) $(gccOptions) VASM.c
Descriptor.o	:	Descriptor.c
	$(gccCompiler) $(gccOptions) Descriptor.c
Bitmap.o	:	Bitmap.asm
	$(AsmCompiler) $(AsmOptions) -o Bitmap.o Bitmap.asm
CPhysical.o	:	$(IncludeDir)/Memory.h CPhysical.c
	$(gccCompiler) $(gccOptions) CPhysical.c
Physical.o	:	Physical.asm
	$(AsmCompiler) $(AsmOptions) -o Physical.o Physical.asm
malloc.o	:	$(IncludeDir)/malloc.h malloc.c
	$(gccCompiler) $(gccOptions) -o malloc.o malloc.c
Heap.o	:	$(IncludeDir)/Heap.h Heap.c
	$(gccCompiler) $(gccOptions) -o Heap.o Heap.c

