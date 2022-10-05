section .bootloader
begin:
  dd 0xe85250d6
  dd 0
  dd end - begin
  dd 0x100000000 - (0xe85250d6 + 0 + end - begin)
  dw 0
  dw 0
  dd 8
end: