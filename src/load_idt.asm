# Author: turrnut
# Copyright © turrnut under the GNU General Public License Version 3
# 
# load_idt.asm
#
.section .text
.global load_idt

# loads the IDT
load_idt:
    mov 4(%esp), %eax   # get the idt_ptr argument
    lidt (%eax)         # load it into the IDT register
    ret
