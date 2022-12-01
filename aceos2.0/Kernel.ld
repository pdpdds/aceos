OUTPUT_FORMAT("pei-i386")

VirtualAddress =  0xC0000000;
PhysicalLoadAddress = 0x100000; 
ENTRY (_KernelEntry)
SECTIONS
{ 
    .text  VirtualAddress : AT( PhysicalLoadAddress )
    { 
        g_code = .; _g_code = .;
        *(.text)
        *(.rodata)
        . = ALIGN( 4096 );                
    }   
    .data : AT( PhysicalLoadAddress + (g_data - g_code))
    { 
        g_data = .; _g_data = .;
        data = .;
        *(.data)
        . = ALIGN(4096);		
    }
    .bss : AT( PhysicalLoadAddress + (g_bss - g_code) )
    { 
        g_bss = .; _g_bss = .;
        sbss = .; _sbss = .;
        bss = .;
        *(.bss)
        *(COMMON)
        . = ALIGN(4096); 
    }
    ebss = .; _ebss = .;
} 
