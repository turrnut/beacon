extern lmstrt
global start
section .text
bits 32
start:
  mov esp, stackstart
  call check
  call page
  lgdt [gdt64.ptr]
  jmp gdt64.cseg :lmstrt
  hlt
page:
  call sppg
  call ebpg
  ret
sppg:
  mov eax, pglth
  or eax, 0b11
  mov [pglfo], eax
  mov eax, pgltw
  or eax, 0b11
  mov [pglfo], eax
  mov ecx, 0
.repeat:
  mov eax, 0x20000
  mul ecx
  or eax, 0b10000011
  mov [pgltw+ecx*8],eax
  inc ecx
  cmp ecx, 512
  jne .repeat
  ret
ebpg:
  mov eax, pglfo
  mov cr3, eax
  mov eax, cr4
  or eax, 1<<5
  mov cr4, eax
  mov ecx, 0xC0000080
  rdmsr
  or eax, 1<<8
  wrmsr
  mov eax, cr0
  or eax, 1<<31
  mov cr0, eax
  ret
check:
  call checkboot
  call checkid
  call checklm
  ret
checkid:
  pushfd
  pop eax
  mov ecx, eax
  xor eax, 1 << 21
  push eax
  popfd
  pushfd
  pop eax
  push ecx
  popfd
  cmp eax, ecx
  je .nid
  ret
.nid:
  mov al, "CI"
checklm:
  mov eax, 0x80000000
  cpuid
  cmp eax, 0x80000001
  jb .nlm
  mov eax, 0x80000001
  cpuid
  test edx, 1 << 29
  jz .nlm
  ret
.nlm
  mov al, "NL"
  ret
checkboot:
  cmp eax, 0x36d76289
  jne .nbooterr
  ret
.nbooterr:
  mov al, "MB"
  jmp errormsg
errormsg:
  mov dword [0xb8000], 0x2f722f722f6f2f722f45
  mov byte [0xb800a], al
  hlt
section .bss
align 4096
pgltw:
  resb 4096
pglth:
  resb 4096
pglfo:
  resb 4096
stackend:
  resb 10384
stackstart:
section .rodata
gdt64:
  dq 0
.cseg: equ $ - gdt64
	dq (1 << 43) | (1 << 44) | (1 << 47) | (1 << 53)
.ptr:
  dw $-gdt64-1
  dq gdt64