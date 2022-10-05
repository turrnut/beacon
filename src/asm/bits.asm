global lmstrt
extern kernel

section .text
bits 64
lmstrt:
    mov ax, 0
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
	    call kernel
    hlt