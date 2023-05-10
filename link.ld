ENTRY(boot)

SECTIONS {
	. = 1M;
	.text :
	{
		*(.multiboot)
		*(.text*)
		*(.rodata)
	}
	.data :
	{
		KEEP(*(.init_array))
		KEEP(*(SORT_BY_INIT_PRIORITY(.init_array.*)))
	}
	.bss : 
	{
		*(.bss)
	}
	/DISCARD/ :
	{
		*(.fini_array*)
		*(.comment)
	}
}
