# Author: turrnut
# Copyright © turrnut under the GNU General Public License Version 3
# 
# i.asm
#

.set IRQ_BASE, 0x20

.section .text

.extern handleInterrupt

.global IgnoreInterruptRequest

.macro HandleException numb
.global HandleException\numb\()
HandleException\numb\():
    movb $\numb, interruptnumber
    jmp interrupt_common
.endm

.macro HandleInterruptRequest numb
.global HandleInterruptRequest\numb\()
HandleInterruptRequest\numb\():
    movb $\numb + IRQ_BASE, interruptnumber
    jmp interrupt_common
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

# Common interrupt handling routine
interrupt_common:

    pusha               # Push all general-purpose registers
    pushl %ds           # Push data segment register
    pushl %es           # Push extra segment register
    pushl %fs           # Push FS register
    pushl %gs           # Push GS register

    # Set segment registers
    movw $0x10, %ax     # Assume kernel data segment is 0x10
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs

    pushl %esp          # Push ESP to stack
    pushl interruptnumber
    call handleInterrupt
    addl $8, %esp       # Clean up the stack (interruptnumber and old ESP)

    # Acknowledge the interrupt to the PICs
    # Check if it is necessary to send EOI to the slave PIC
    cmpb $0x28, interruptnumber
    jb no_slave_eoi
    movb $0x20, %al     # EOI command
    outb %al, $0xA0     # Send EOI to slave PIC

no_slave_eoi:
    movb $0x20, %al     # EOI command
    outb %al, $0x20     # Send EOI to master PIC

    popl %gs            # Restore GS
    popl %fs            # Restore FS
    popl %es            # Restore ES
    popl %ds            # Restore DS
    popa                # Restore all general-purpose registers

    iret                # Return from interrupt

IgnoreInterruptRequest:
    iret

.data
    interruptnumber: .byte 0
