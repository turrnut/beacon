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
    movb $\numb, (interruptnumber)
    jmp interrupt_bottom
.endm

.macro HandleInterruptRequest numb
.global HandleInterruptRequest\numb\()
HandleInterruptRequest\numb\():
    movb $\numb + IRQ_BASE, (interruptnumber)
    jmp interrupt_bottom
.endm

HandleInterruptRequest 0x00
HandleInterruptRequest 0x01

interrupt_bottom:

    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interruptnumber)
    call handleInterrupt

    # addl $5, %esp
    movl %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

IgnoreInterruptRequest:

    iret

.data
    interruptnumber: .byte 0
