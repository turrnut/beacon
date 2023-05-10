# Author: turrnut
# Copyrighted © turrnut under the Apache 2.0 license
#
# We hoped that you will use this piece of open source
# software fairly. read the LICENSE for details for
# more details about how you can use it, you have freedom
# to distribute and use this file in your project. However,
# you will have to state changes you made and include the
# orginal author of this file.
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
