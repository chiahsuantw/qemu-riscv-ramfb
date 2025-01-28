SECTIONS
{
	. = 0x80200000;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
