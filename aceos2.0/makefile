#                   Ace OS makefile for gcc 3.1 
#				- Created by Sam (samueldotj@gmail.com) 
#------------------------------------------------------------------------------------------------
KernelObjects = Kernel.o Processor.o CProcessor.o V86.o Timer.o CTimer.o RTC.o CKernel.o IO.o \
		TaskMan.o Thread.o ObjMan.o Scheduler.o SysInfo.o IPC.o\
		GSH.o Harddisk.o Partition.o GFS.o FAT.o VFS.o \
		List.o VGAText.o Printer.o printf.o Beep.o Util.o Keyboard.o CKeyboard.o \
		SysCall.o SysCallInt.o PE.o Environment.o
		# DeviceDriver.o  \
		# CFloppy.o Floppy.o   \
		#  VESAInt.o DMA.o 
		#PCIClassInfo.o PCI.o  
ASM_COMPILER = nasm
GCC_COMPILER = gcc
GXX_COMPILER = g++
INCLUDE_DIR = Include
ASM_OPTIONS = -i $(INCLUDE_DIR) -w+orphan-labels -f win32
GCC_OPTIONS = -D KERNELBUILT -nostartfiles -ffreestanding -fno-use-cxa-atexit -c  -I $(INCLUDE_DIR) -Wall -m32
GXX_OPTIONS = -D KERNELBUILT -nostartfiles -fno-exceptions -fno-use-cxa-atexit -ffreestanding -c  -I $(INCLUDE_DIR)  -Wall  -m32
IMPDEF = ..\impdef.exe
LibDir = C:\msys64\mingw32\lib\gcc\i686-w64-mingw32\11.2.0
LibDir2 = C:\msys64\home\juhan\libc

Memory = $(patsubst %.c,%.o,$(wildcard Memory/*.c)) $(patsubst %.asm,%.o,$(wildcard Memory/*.asm))
String = $(patsubst %.c,%.o,$(wildcard String/*.c))

#------------------------------------------------------------------------------------------------
#ld -Ttext 0xC0000000 --image 0xC0000000 --shared -entry _KernelEntry -nostartupfiles -o Kernel.sys $(KernelObjects) $(wildcard String/*.o) $(wildcard Memory/*.o) -LC:\MinGW\lib\gcc\mingw32\4.5.2 -LC:\MinGW\lib -lmingwex -lgcc	
Kernel	:	String Memory $(KernelObjects)
	ld -m  i386pe -T Kernel.ld --file-alignment 0x1000 --section-alignment 0x1000 \
		-o Kernel.sys $(KernelObjects) $(wildcard String/*.o) $(wildcard Memory/*.o) \
		-L$(LibDir) -L$(LibDir2) -lgcc -lc
	
Kernel.o	:	Kernel.asm
	$(ASM_COMPILER) $(ASM_OPTIONS) -o Kernel.o Kernel.asm
CKernel.o	:	$(INCLUDE_DIR)/Processor.h CKernel.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o CKernel.o CKernel.c
Processor.o	:	$(INCLUDE_DIR)/Common.inc Processor.asm
	$(ASM_COMPILER) $(ASM_OPTIONS) -o Processor.o Processor.asm
CProcessor.o	:	$(INCLUDE_DIR)/Processor.h CProcessor.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o CProcessor.o CProcessor.c
PE.o	:	$(INCLUDE_DIR)/PE.h PE.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o PE.o PE.c
ObjMan.o	:	$(INCLUDE_DIR)/ObjMan.h $(INCLUDE_DIR)/ObjMan.h ObjMan.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o ObjMan.o ObjMan.c
TaskMan.o	:	$(INCLUDE_DIR)/TaskMan.h $(INCLUDE_DIR)/Thread.h TaskMan.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o TaskMan.o TaskMan.c
Thread.o	:	$(INCLUDE_DIR)/Thread.h $(INCLUDE_DIR)/TaskMan.h Thread.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Thread.o Thread.c
Scheduler.o	:	$(INCLUDE_DIR)/Scheduler.h $(INCLUDE_DIR)/TaskMan.h $(INCLUDE_DIR)/Thread.h Scheduler.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Scheduler.o Scheduler.c
Keyboard.o	:	$(INCLUDE_DIR)/Common.inc Keyboard.asm 
	$(ASM_COMPILER) $(ASM_OPTIONS) -o Keyboard.o Keyboard.asm
CKeyboard.o	:	$(INCLUDE_DIR)/Keyboard.h CKeyboard.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o CKeyboard.o CKeyboard.c
VGAText.o	:	IO.o $(INCLUDE_DIR)/VGAText.h VGAText.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o VGAText.o VGAText.c
IO.o		:	IO.c $(INCLUDE_DIR)/IO.h
	$(GCC_COMPILER) $(GCC_OPTIONS) -o IO.o IO.c
DMA.o		:	DMA.c $(INCLUDE_DIR)/DMA.h
	$(GCC_COMPILER) $(GCC_OPTIONS) -o DMA.o DMA.c
SysCall.o	:	SysCall.c $(INCLUDE_DIR)/SysCall.h
	$(GCC_COMPILER) $(GCC_OPTIONS) -o SysCall.o SysCall.c
SysCallInt.o	:	SysCallInt.asm
	$(ASM_COMPILER) $(ASM_OPTIONS) -o SysCallInt.o SysCallInt.asm
Timer.o		:	Timer.asm
	$(ASM_COMPILER) $(ASM_OPTIONS) -o Timer.o Timer.asm
CTimer.o	:	IO.o $(INCLUDE_DIR)/Timer.h CTimer.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o CTimer.o CTimer.c
PCIClassInfo.o		:	$(INCLUDE_DIR)/PCI.h PCIClassInfo.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o PCIClassInfo.o PCIClassInfo.c
PCI.o		:	IO.o $(INCLUDE_DIR)/PCI.h PCI.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o PCI.o PCI.c
RTC.o	:	IO.o $(INCLUDE_DIR)/RTC.h RTC.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o RTC.o RTC.c
SysInfo.o	:	$(INCLUDE_DIR)/SysInfo.h SysInfo.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o SysInfo.o SysInfo.c
Harddisk.o	:	GSH.o $(INCLUDE_DIR)/Harddisk.h Harddisk.c
	$(GXX_COMPILER) $(GXX_OPTIONS) -o Harddisk.o Harddisk.c
Floppy.o		:	Floppy.asm
	$(ASM_COMPILER) $(ASM_OPTIONS) -o Floppy.o Floppy.asm
CFloppy.o	:	GSH.o $(INCLUDE_DIR)/Floppy.h CFloppy.c
	$(GXX_COMPILER) $(GXX_OPTIONS) -o CFloppy.o CFloppy.c
GSH.o		:	$(INCLUDE_DIR)/GSH.h GSH.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o GSH.o GSH.c
GFS.o		:	$(INCLUDE_DIR)/GFS.h GFS.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o GFS.o GFS.c
IPC.o		:	$(INCLUDE_DIR)/IPC.h IPC.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o IPC.o IPC.c
DeviceDriver.o	:	$(INCLUDE_DIR)/DeviceDriver.h DeviceDriver.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o DeviceDriver.o DeviceDriver.c
Beep.o		:	Beep.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Beep.o Beep.c
Partition.o	:	$(INCLUDE_DIR)/Partition.h Partition.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Partition.o Partition.c
FAT.o		:	$(INCLUDE_DIR)/FAT.h FAT.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o FAT.o FAT.c
VFS.o		:	$(INCLUDE_DIR)/VFS.h VFS.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o VFS.o VFS.c
List.o		:	$(INCLUDE_DIR)/List.h List.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o List.o List.c
printf.o	:	$(INCLUDE_DIR)/printf.h printf.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o printf.o printf.c
Printer.o	:	$(INCLUDE_DIR)/Printer.h Printer.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Printer.o Printer.c
VESAInt.o	:	$(INCLUDE_DIR)/VESAInt.h VESAInt.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o VESAInt.o VESAInt.c
V86.o		:	$(INCLUDE_DIR)/V86.h V86.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o V86.o V86.c
Environment.o		:	$(INCLUDE_DIR)/Environment.h Environment.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Environment.o Environment.c
Util.o		:	$(INCLUDE_DIR)/Util.h Util.c
	$(GCC_COMPILER) $(GCC_OPTIONS) -o Util.o Util.c

Memory	:	ALWAYSRUN
	$(MAKE) -C Memory
String	:	ALWAYSRUN
	$(MAKE) -C String
ALWAYSRUN:

.PHONY	:	clean
clean	:
	rm -rf *.o
	rm -rf *.bin
	rm -rf *.exe