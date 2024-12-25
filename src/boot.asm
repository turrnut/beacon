# Author: turrnut
# Copyright © turrnut under the GNU General Public License Version 3
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
.global boot
boot:
    mov $stack, %esp
    push %eax
    push %ebx
    call start
    cli
    hlt
    jmp boot
.section .bss
.space 2097152 # 2 MiB
stack:
