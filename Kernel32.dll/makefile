#                   Ace OS Kernel Interface makefile for gcc 3.1 
#				- Created by Sam (samuelhard@yahoo.com) 
#------------------------------------------------------------------------------------------------
OBJECT_FILES = Kernel32.o
ASM_COMPILER = nasmw
GCC_COMPILER = gcc
GXX_COMPILER = g++
INCLUDE_DIR = -I Include -I ..\Kernel\Include
ASM_OPTIONS = -i $(INCLUDE_DIR) -w+orphan-labels -f win32
GCC_OPTIONS = -m32 -D KERNELBUILT -nostartfiles -ffreestanding -c  $(INCLUDE_DIR) -Wall
GXX_OPTIONS = -m32 -D KERNELBUILT -nostartfiles -fno-exceptions -ffreestanding -c  $(INCLUDE_DIR)  -Wall
IMPDEF = .\dll2def.exe
DLLTOOL = dlltool.exe
Kernel32	:	$(OBJECT_FILES)
# set path=%PATH%;..\..\

	ld -m  i386pe -Ttext 0xC0000000 --image 0xC0000000 --shared -entry _DllMain -o Kernel32.dll $(OBJECT_FILES)  -LC:\msys64\mingw32\lib\gcc\i686-w64-mingw32\11.2.0 -LC:\msys64\mingw32\lib -lmingwex -lgcc
	$(IMPDEF) Kernel32.dll Kernel32.def
	$(DLLTOOL) --dllname Kernel32.dll --def Kernel32.def --output-lib libKernel32.a
	
Kernel32.o	:	Include/Kernel32.h Kernel32.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Kernel32.o Kernel32.c
