OUTPUT_FORMAT("binary")
ENTRY(loader)

SECTIONS
{
	.text 0x100000 : /* Kernel starts at 1 MB */
	{
		text = .;
		*(.text*)
		. = ALIGN(0x1000);
		rodata = .;
		*(.rodata*)
	}

	.data ALIGN(0x1000) : AT(ADDR(.data))
	{
		data = .;
		*(.data*)
		endOfKernelBinary = .;
	}

	.bss ALIGN(0x1000) : AT(ADDR(.bss))
	{
		bss = .;
		*(.bss*)
		*(EXCLUDE_FILE (*.o) COMMON)
	}

	. = ALIGN(0x1000);
	endOfKernel = .;

	/* Heap section */
	. = ALIGN(0x1000);
	.heap :
	{
		_heap_start = .;
		. = . + 0x400000; /* 4 MB heap */
		_heap_end = .;
	}
}
