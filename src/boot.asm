# Author: turrnut
# Copyrighted © turrnut under the Apache 2.0 license
# We hope that you will utilize this open-source software
# responsibly. Please read the LICENSE for comprehensive
# information on how you can employ it. You have the
# freedom to distribute and incorporate this file into your
# project. However, you must indicate any modifications
# you've made and provide credit to the original author of
# this file.
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
