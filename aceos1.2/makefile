#                   Ace OS makefile for gcc 3.1 
#				- Created by Sam (samuelhard@yahoo.com) 
#------------------------------------------------------------------------------------------------
KernelObjects = Kernel.o Processor.o Timer.o CTimer.o CKernel.o  MemMan.o Heap.o DeviceDriver.o IO.o TMan.o TaskMan.o \
		MemMove.o VGAText.o VGACursor.o Keyboard.o CKeyboard.o PIC.o PCI.o RTC.o GSH.o Harddisk.o \
		HMess.o Printf.o Partition.o FAT.o String.o Beep.o 
AsmCompiler = nasm
gccCompiler = gcc
gxxCompiler = g++
AsmOptions = -i Includes -w+orphan-labels -f win32
gccoptions = -m32 -fno-use-cxa-atexit -D KERNELBUILT -nostartfiles -ffreestanding -c  -I Includes
gxxoptions = -m32 -fno-use-cxa-atexit -D KERNELBUILT -nostartfiles -fno-exceptions -ffreestanding -c  -I Includes 
LibDir = C:\msys64\mingw32\lib\gcc\i686-w64-mingw32\11.2.0
LibDir2 = ..\libc
#------------------------------------------------------------------------------------------------
.PHONY	:	all
all	:	Kernel Boot	
.PHONY	:	Boot
Boot	:	Boot/Boot.asm Boot/OSLoader.asm Boot/MakeBoot.c 
	nasm -i Includes -w+orphan-labels -o Boot.bin Boot/Boot.asm
	nasm -i Includes -w+orphan-labels -o OSLoader.bin Boot/OSLoader.asm
	#gcc -m32 -o MakeBoot.exe Boot/MakeBoot.c
.PHONY	:	clean
clean	:
	rm -rf *.o
	rm -rf *.bin
	rm -rf *.exe

Kernel	:	$(KernelObjects)
	ld -m  i386pe -Ttext 0x19000 -Tdata 0x258c0 --image 0x19000 --shared -entry _KernelEntry -o Kernel.exe $(KernelObjects) -L$(LibDir) -L$(LibDir2) -lgcc -lc
#	ld -m  i386pe -Ttext 0x19000 --image 0x19000 --dll --shared --no-gc-sections --no-undefined -o Kernel.dll $(KernelObjects) -L$(LibDir) -L$(LibDir2) -lgcc -lc
Kernel.o	:	Kernel/Kernel.asm
	$(AsmCompiler) $(AsmOptions) -o Kernel.o Kernel/Kernel.asm
CKernel.o	:	Includes/Collect.h Includes/Processor.h Kernel/CKernel.cpp
	$(gxxCompiler) $(gxxoptions) -o CKernel.o Kernel/CKernel.cpp
Processor.o	:	Includes/Common.inc Kernel/Processor.asm
	$(AsmCompiler) $(AsmOptions) -o Processor.o Kernel/Processor.asm
MemMan.o	:	Includes/Common.inc Kernel/MemMan.asm
	$(AsmCompiler) $(AsmOptions) -o MemMan.o Kernel/MemMan.asm
TMan.o	:		Includes/Common.inc Kernel/TMan.asm
	$(AsmCompiler) $(AsmOptions) -o TMan.o Kernel/TMan.asm
TaskMan.o	:	Heap.o Kernel/TaskMan.cpp
	$(gxxCompiler) $(gxxoptions) -o TaskMan.o Kernel/TaskMan.cpp
Heap.o		:	MemMan.o Includes/Heap.h  Kernel/Heap.c
	$(gccCompiler) $(gccoptions) -o Heap.o Kernel/Heap.c
Keyboard.o	:	Includes/HMess.inc Includes/Common.inc Kernel/Keyboard.asm 
	$(AsmCompiler) $(AsmOptions) -o Keyboard.o Kernel/Keyboard.asm
CKeyboard.o	:	Kernel/CKeyboard.c
	$(gccCompiler) $(gccoptions) -o CKeyboard.o Kernel/CKeyboard.c
VGAText.o	:	IO.o Includes/Common.inc Includes/VGAText.inc Kernel/VGAText.asm
	$(AsmCompiler) $(AsmOptions) -o VGAText.o Kernel/VGAText.asm
VGACursor.o	:	IO.o Kernel/VGACursor.c
	$(gccCompiler) $(gccoptions) -o VGACursor.o Kernel/VGACursor.c
PIC.o		:	Kernel/PIC.asm
	$(AsmCompiler) $(AsmOptions) -o PIC.o Kernel/PIC.asm
IO.o		:	Kernel/IO.asm
	$(AsmCompiler) $(AsmOptions) -o IO.o Kernel/IO.asm
Timer.o		:	Kernel/Timer.asm
	$(AsmCompiler) $(AsmOptions) -o Timer.o Kernel/Timer.asm
CTimer.o	:	IO.o Includes/Timer.h Kernel/CTimer.c
	$(gccCompiler) $(gccoptions) -o CTimer.o Kernel/CTimer.c
PCI.o		:	IO.o Includes/PCI.h Kernel/PCI.cpp
	$(gxxCompiler) $(gxxoptions) -o PCI.o Kernel/PCI.cpp
RTC.o		:	IO.o Includes/RTC.h Kernel/RTC.c
	$(gccCompiler) $(gccoptions) -o RTC.o Kernel/RTC.c
Printf.o	:	VGAText.o VGACursor.o Kernel/Printf.c
	$(gccCompiler) $(gccoptions) -c -I Includes -o Printf.o Kernel/Printf.c
MemMove.o	:	Kernel/MemMove.asm
	$(AsmCompiler) $(AsmOptions) -o MemMove.o Kernel/MemMove.asm
String.o	:	Kernel/String.c
	$(gccCompiler) $(gccoptions) -o String.o Kernel/String.c
Harddisk.o	:	GSH.o Includes/Harddisk.h Kernel/Harddisk.cpp
	$(gxxCompiler) $(gxxoptions) -o Harddisk.o Kernel/Harddisk.cpp
GSH.o		:	Includes/GSH.h Kernel/GSH.cpp
	$(gxxCompiler) $(gxxoptions) -o GSH.o Kernel/GSH.cpp
HMess.o		:	Includes/HMess.h Kernel/HMess.c
	$(gccCompiler) $(gccoptions) -o HMess.o Kernel/HMess.c
DeviceDriver.o	:	Includes/DeviceDriver.h Kernel/DeviceDriver.cpp
	$(gxxCompiler) $(gxxoptions) -o DeviceDriver.o Kernel/DeviceDriver.cpp
Beep.o		:	Kernel/Beep.c
	$(gccCompiler) $(gccoptions) -o Beep.o Kernel/Beep.c
Mouse.o		:	Includes/HMess.inc Includes/Common.inc Kernel/Mouse.asm 
	$(AsmCompiler) $(AsmOptions) -o Mouse.o Kernel/Mouse.asm
CMouse.o	:	Includes/Mouse.h Kernel/CMouse.c
	$(gccCompiler) $(gccoptions) -o CMouse.o Kernel/CMouse.c
Partition.o	:	Includes/Partition.h Kernel/Partition.cpp
	$(gxxCompiler) $(gxxoptions) -o Partition.o Kernel/Partition.cpp
FAT.o		:	Includes/FAT.h Kernel/FAT.cpp
	$(gxxCompiler) $(gxxoptions) -o FAT.o Kernel/FAT.cpp
