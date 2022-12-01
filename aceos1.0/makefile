#                   Ace OS makefile for gcc 3.1 
#				- Created by Sam (samuelhard@yahoo.com) 
#------------------------------------------------------------------------------------------------
KernelObjects = Kernel.o Processor.o Timer.o  CKernel.o  MemMan.o Heap.o DeviceDriver.o IO.o MemMove.o \
		VGAText.o VGACursor.o Keyboard.o CKeyboard.o PIC.o PCI.o RTC.o GSH.o Harddisk.o \
		HMess.o Beep.o Printf.o  Mouse.o CMouse.o Partition.o FAT.o NumConvert.o String.o
LibDir = C:\msys64\mingw32\lib\gcc\i686-w64-mingw32\11.2.0
LibDir2 = C:\msys64\home\juhan\libc

Kernel	:	$(KernelObjects)
	ld  -m  i386pe -Ttext 0x19000  -o Kernel.bin $(KernelObjects) -L$(LibDir) -L$(LibDir2) -lc
.PHONY	:	All
all	:	Kernel Boot	
.PHONY	:	Boot
Boot	:	Boot/Boot.asm Boot/OSLoader.asm Boot/MakeBoot.c 
	nasm -i Includes -w+orphan-labels -o Boot.bin Boot/Boot.asm
	nasm -i Includes -w+orphan-labels -o OSLoader.bin Boot/OSLoader.asm
	#gcc -m32 -o MakeBoot.exe Boot/MakeBoot.c
.PHONY	:	Clean
clean	:
	rm -rf *.o
	rm -rf *.bin
	rm -rf *.exe
Kernel.o	:	Kernel/Kernel.asm
	nasm -i Includes -w+orphan-labels -f coff -o Kernel.o Kernel/Kernel.asm
CKernel.o	:	Includes/Collect.h Includes/Processor.h Kernel/CKernel.cpp
	g++ -fno-use-cxa-atexit -m32 -o2 -fno-exceptions  -ffreestanding -c  -I Includes -o CKernel.o Kernel/CKernel.cpp
Processor.o	:	Includes/Common.inc Kernel/Processor.asm
	nasm -i Includes -w+orphan-labels -f coff -o Processor.o Kernel/Processor.asm
MemMan.o	:	Includes/Common.inc Kernel/MemMan.asm
	nasm -i Includes -w+orphan-labels -f coff -o MemMan.o Kernel/MemMan.asm
Heap.o		:	MemMan.o Includes/Heap.h  Kernel/Heap.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o Heap.o Kernel/Heap.c
Keyboard.o	:	Includes/HMess.inc Includes/Common.inc Kernel/Keyboard.asm 
	nasm -i Includes -w+orphan-labels -f coff -o Keyboard.o Kernel/Keyboard.asm
CKeyboard.o	:	Kernel/CKeyboard.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o CKeyboard.o Kernel/CKeyboard.c
VGAText.o	:	IO.o Includes/Common.inc Includes/VGAText.inc Kernel/VGAText.asm
	nasm -i Includes -w+orphan-labels -f coff -o VGAText.o Kernel/VGAText.asm
VGACursor.o	:	IO.o Kernel/VGACursor.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o VGACursor.o Kernel/VGACursor.c
PIC.o		:	Kernel/PIC.asm
	nasm -i Includes -w+orphan-labels -f coff -o PIC.o Kernel/PIC.asm
IO.o		:	Kernel/IO.asm
	nasm -i Includes -w+orphan-labels -f coff -o IO.o Kernel/IO.asm
Timer.o		:	Kernel/Timer.asm
	nasm -i Includes -w+orphan-labels -f coff -o Timer.o Kernel/Timer.asm
PCI.o		:	IO.o Includes/PCI.h Kernel/PCI.cpp
	gcc -fno-use-cxa-atexit -m32 -fno-exceptions -ffreestanding -c  -I Includes -o PCI.o Kernel/PCI.cpp
RTC.o		:	IO.o Includes/RTC.h Kernel/RTC.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o RTC.o Kernel/RTC.c
Printf.o	:	VGAText.o VGACursor.o Kernel/Printf.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c -I Includes -o Printf.o Kernel/Printf.c
MemMove.o	:	Kernel/MemMove.asm
	nasm -i Includes -w+orphan-labels -f coff -o MemMove.o Kernel/MemMove.asm
String.o	:	Kernel/String.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o String.o Kernel/String.c
Harddisk.o	:	GSH.o Includes/Harddisk.h Kernel/Harddisk.cpp
	g++ -m32 -fno-exceptions -ffreestanding -c  -I Includes -o Harddisk.o Kernel/Harddisk.cpp
GSH.o	:	Includes/GSH.h Kernel/GSH.cpp
	g++ -m32 -fno-exceptions -ffreestanding -c  -I Includes -o GSH.o Kernel/GSH.cpp
HMess.o	:	Includes/HMess.h Kernel/HMess.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o HMess.o Kernel/HMess.c
DeviceDriver.o	:	Includes/DeviceDriver.h Kernel/DeviceDriver.cpp
	g++ -fno-use-cxa-atexit -m32 -fno-exceptions -ffreestanding -c  -I Includes -o DeviceDriver.o Kernel/DeviceDriver.cpp
Beep.o	:	Kernel/Beep.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o Beep.o Kernel/Beep.c
Mouse.o	:	Includes/HMess.inc Includes/Common.inc Kernel/Mouse.asm 
	nasm -i Includes -w+orphan-labels -f coff -o Mouse.o Kernel/Mouse.asm
CMouse.o	:	Includes/Mouse.h Kernel/CMouse.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o CMouse.o Kernel/CMouse.c
Partition.o	:	Includes/Partition.h Kernel/Partition.cpp
	g++ -fno-use-cxa-atexit -m32 -fno-exceptions -ffreestanding -c  -I Includes -o Partition.o Kernel/Partition.cpp
FAT.o	:	Includes/FAT.h Kernel/FAT.cpp
	g++ -fno-use-cxa-atexit -m32 -fno-exceptions -ffreestanding -c  -I Includes -o FAT.o Kernel/FAT.cpp
NumConvert.o	:	Kernel/NumConvert.c
	gcc -fno-use-cxa-atexit -m32 -ffreestanding -c  -I Includes -o NumConvert.o Kernel/NumConvert.c

