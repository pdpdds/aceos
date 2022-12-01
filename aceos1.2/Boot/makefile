AsmCompiler = nasm
gccCompiler = c:\djgpp\bin\gcc
gxxCompiler = c:\djgpp\bin\g++
AsmOptions = -i Includes -w+orphan-labels -f coff
gccoptions = -ffreestanding -c  -I Includes
gxxoptions = -fno-exceptions -ffreestanding -c  -I Includes 
#------------------------------------------------------------------------------------------------
Boot	:	Boot.asm OSLoader.asm MakeBoot.c 
	$(AsmCompiler) -i Includes -w+orphan-labels -o Boot.bin Boot.asm
	$(AsmCompiler) -i Includes -w+orphan-labels -o OSLoader.bin OSLoader.asm
	$(gccCompiler) -o MakeBoot.exe MakeBoot.c
	
