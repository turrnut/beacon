# Copyright (c) Turrnut Open Source Organization
# Under the GPL v3 License
# See COPYING for information on how you can use this file
#
# boot.asm
#
.section .data
scancode_to_char:
    .byte 0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', 0
    .byte 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0, 0, 'a', 's'
    .byte 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c'
    .byte 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
    .fill 0x4B - 0x3A, 1, 0         # pad unused scancodes before left arrow
    .byte -3                        # left arrow
    .fill 0x4D - 0x4C, 1, 0         # pad unused scancode before right arrow
    .byte -4                        # right arrow
    .fill 0x48 - 0x4E, 1, 0         # pad unused scancodes before up arrow
    .byte -1                        # up arrow
    .fill 0x50 - 0x49, 1, 0         # pad unused scancode before down arrow
    .byte -2                        # down arrow
    .fill 255 - 0x51, 1, 0          # pad remaining scancode entries with 0


.global scancode_buffer
scancode_buffer:
    .byte 0                          # holds the last valid scancode

.section .text
.global keyboard_handler
.global get_key

keyboard_handler:
    pusha                        # Save all registers

    inb $0x60, %al               # Read scancode from keyboard port
    test $0x80, %al              # Check if the scancode is a key release
    jnz skip_handler             # Skip releases (we only care about keypresses)

    movzbl %al, %eax             # Zero-extend scancode to 32 bits
    cmp $58, %al                 # Check if scancode is within valid range (you could adjust this range)
    ja skip_handler              # Skip invalid scancodes

    # Add the scancode to buffer
    pushl %eax                   # Push scancode onto stack for C function
    call handle_keypress         # Call the C function
    add $4, %esp                 # Clean up stack

skip_handler:
    popa                         # Restore registers
    movb $0x20, %al              # Send end-of-interrupt to PIC
    outb %al, $0x20
    iret                         # Return from interrupt


get_key:
    inb $0x60, %al                   # read from keyboard port
    ret                              # return the scancode
