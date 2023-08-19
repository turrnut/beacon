gccparams = -m32 -nostdlib -fno-builtin -fno-exceptions -fno-leading-underscore
asmparams = --32
ldparams = -melf_i386 -s

objs = obj/boot.o obj/os.o obj/console.o obj/gdt.o

compile:
	rm -rf out
	rm -rf obj
	mkdir out
	mkdir obj
	rm -rf $(objs)
	as $(asmparams) -o obj/boot.o src/boot.asm
	gcc $(gccparams) -o obj/gdt.o -c src/gdt.c
	gcc $(gccparams) -o obj/console.o -c src/console.c
	
	gcc $(gccparams) -o obj/os.o -c src/os.c

	ld $(ldparams) -T link.ld -o out/os.bin $(objs)
	cp out/os.bin build/boot/os.bin
	grub-mkrescue --output=out/os.iso build
