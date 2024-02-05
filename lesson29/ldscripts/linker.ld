FLASH_START = 0x08000000;
SRAM_START = 0x20000000;

FLASH_SIZE = 64K;
SRAM_SIZE = 20K;

ISR_SIZE = 512;
STACK_SIZE = 1K;
HEAP_SIZE = 1K;

MEMORY
{
    ISR (r)     : ORIGIN = FLASH_START, LENGTH = ISR_SIZE
    FLASH (rx)  : ORIGIN = FLASH_START + ISR_SIZE, LENGTH = FLASH_SIZE - ISR_SIZE
    STACK (rw)  : ORIGIN = SRAM_START, LENGTH = STACK_SIZE
    RAM (rw)    : ORIGIN = SRAM_START + STACK_SIZE, LENGTH = SRAM_SIZE - STACK_SIZE
}

__StackTop = ORIGIN(STACK) + LENGTH(STACK);

SECTIONS
{
    .vectors :
    {
        KEEP(*(.vectors));
        . = ALIGN(4);
    } > ISR

    .text :
    {
        *(.text)
        *(.rodata)
        . = ALIGN(4);
        __etext = .;
    } > FLASH
    
    .data :
    {
        __data_start__ = .;
        *(.data)
        __data_end__ = .;
        . = ALIGN(4);
    } > RAM AT > FLASH
    
    .bss :
    {
        __bss_start__ = .;
        *(.bss)
        __bss_end__ = .;
        . = ALIGN(4);
    } > RAM
    
    end = . + HEAP_SIZE;
    
    copy.table :
    {
        __copy_table_start__ = .;
        LONG (__etext)
        LONG (__data_start__)
        LONG (__data_end__ - __data_start__)
        __copy_table_end__ = .;
        . = ALIGN(4);
    } > FLASH
    
    .zero.table :
    {
        __zero_table_start__ = .;
        LONG (__bss_start__)
        LONG (__bss_end__ - __bss_start__)
        __zero_table_end__ = .;
        . = ALIGN(4);
    } > FLASH
}
