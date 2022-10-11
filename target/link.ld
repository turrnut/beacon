ENTRY(entry)
SECTIONS
{
    . = 1M;
    .boot :
    {
        KEEP(*(.bootloader))
    }
    .text :
    {
        *(.text)
    }
}