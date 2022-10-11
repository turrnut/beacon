asm_src := $(shell find src/asm/ -name *.asm)
asm_obj := $(patsubst src/asm/%.asm, build/asm/%.o, $(asm_src))

cpp_src := $(shell find src/c/ -name *.cpp)
cpp_obj := $(patsubst src/c/%.cpp, build/c/%.o, $(cpp_src))

c_src := $(shell find src/c/ -name *.c)
c_obj := $(patsubst src/c/%.c, build/c/%.o, $(c_src))

$(cpp_obj): build/c/%.o : src/c/%.cpp
	mkdir -p $(dir $@) && \
	g++ -c -ffreestanding $(patsubst build/c/%.o, src/c/%.cpp, $@) -o $@
$(c_obj): build/c/%.o : src/c/%.c
	mkdir -p $(dir $@) && \
	gcc -c -ffreestanding $(patsubst build/c/%.o, src/c/%.c, $@) -o $@
$(asm_obj): build/asm/%.o : src/asm/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/asm/%.o, src/asm/%.asm, $@) -o $@

.PHONY: build
build: $(cpp_obj) $(asm_obj) $(c_obj)
	mkdir -p out && \
	x86_64-elf-ld -n -o out/os.bin -T target/link.ld $(cpp_obj) $(asm_obj) $(c_obj) && \
	cp out/os.bin target/boot/os.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o out/os.iso target