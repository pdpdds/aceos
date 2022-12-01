#                   Ace OS DDK makefile for mingw
#				- Created by Sam (samuelhard@yahoo.com) 
#------------------------------------------------------------------------------------------------
AsmCompiler = nasmw
gccCompiler = gcc
gxxCompiler = gxx
AsmOptions = -i Includes -w+orphan-labels -f coff
gccoptions = -ffreestanding -c  -I Includes
gxxoptions = -fno-exceptions -ffreestanding -c  -I Includes 
#------------------------------------------------------------------------------------------------
.PHONY	:	Boot
Boot	:	Boot/Boot.asm Boot/OSLoader.asm Boot/MakeBoot.c 
.PHONY	:	Clean
Clean	:
	del *.o
	del *.bin
	del *.exe
#------------------------------------------------------------------------------------------------
Kernel.o	:	Kernel/Kernel.asm
	$(AsmCompiler) $(AsmOptions) -o Kernel.o Kernel/Kernel.asm
CKernel.o	:	Includes/Collect.h Includes/Processor.h Kernel/CKernel.cpp
	$(gxxCompiler) -o2 -fno-exceptions  -ffreestanding -c  -I Includes -o CKernel.o Kernel/CKernel.cpp
Processor.o	:	Includes/Common.inc Kernel/Processor.asm
	$(AsmCompiler) $(AsmOptions) -o Processor.o Kernel/Processor.asm
MemMan.o	:	Includes/Common.inc Kernel/MemMan.asm
	$(AsmCompiler) $(AsmOptions) -o MemMan.o Kernel/MemMan.asm
