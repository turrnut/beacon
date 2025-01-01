# Copyright (c) Turrnut Open Source Organization
# Under the GPL v3 License
# See COPYING for information on how you can use this file
#
# boot.asm
#

.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
    .long MAGIC
    .long FLAGS
    .long CHECKSUM

.section .text
.extern start
.extern keyboard_handler
.global boot

boot:
    mov $stack, %esp           # Set up stack pointer

    # Remap PIC
    mov $0x11, %al
    outb %al, $0x20
    outb %al, $0xA0
    mov $0x20, %al
    outb %al, $0x21
    mov $0x28, %al
    outb %al, $0xA1
    mov $0x04, %al
    outb %al, $0x21
    mov $0x02, %al
    outb %al, $0xA1
    mov $0x01, %al
    outb %al, $0x21
    outb %al, $0xA1

    # Mask everything except IRQ1
    movb $0xFD, %al            # Mask all except IRQ1 (bit 1 = 0)
    outb %al, $0x21            # Master PIC
    movb $0xFF, %al            # Mask all slave IRQs
    outb %al, $0xA1            # Slave PIC

    # Set ISR for IRQ1 (this part is still basically useless, but keep it)
    lea idt_table, %eax
    mov $irq1_handler_entry, %ebx
    mov %bx, (%eax)
    shr $16, %ebx
    mov %bx, 6(%eax)
    mov $0x08, 2(%eax)
    movb $0x8E, 4(%eax)

    # Populate rest of IDT with garbage (or placeholders)
    lea idt_table, %eax
    mov $256, %ecx
idt_loop:
    mov $0, (%eax)
    add $8, %eax
    loop idt_loop

    # Load IDT
    lea idt_descriptor, %eax
    lidt (%eax)

    # Disable interrupts entirely to prevent triple fault
    cli

    # Jump to C kernel (start the kernel)
    call start

    hlt
    jmp boot

.section .data
idt_table:
    .space 256 * 8

idt_descriptor:
    .word 256 * 8 - 1
    .long idt_table

.section .bss
.space 2097152  # 2 MiB stack
stack:

.section .text
irq1_handler_entry:
    # Skip actual IRQ1 handling - just make a placeholder
    ret

