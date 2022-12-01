#                   Std C Lib for Ace OS - makefile for gcc 3.1 
#				- Created by Sam (samueldotj@yahoo.co.in) 
#------------------------------------------------------------------------------------------------
ASM_COMPILER = nasmw
GCC_COMPILER = gcc
GXX_COMPILER = gxx
AR = ar
INCLUDE_DIR = ../Include

ASM_OPTIONS = -i $(INCLUDE_DIR) -w+orphan-labels -f win32
GCC_OPTIONS = -fno-exceptions -c -I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/OS 
GXX_OPTIONS = -fno-exceptions -c  -I $(INCLUDE_DIR) -I $(INCLUDE_DIR)/OS 
#-nostdlib
Test	:	Test.o 
	ld  -o Test.exe Test.o --shared  -L ..\..\Kernel32.dll -L..  -lc -lKernel32 -LC:\MinGW\lib\gcc-lib\mingw32\3.2.3 -lgcc
#-entry=_main
Test.o	:	Test.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Test.o Test.c
