asm_src := $(shell find src/asm/ -name *.asm)
asm_obj := $(patsubst src/asm/%.asm, build/asm/%.o, $(asm_src))

c_src := $(shell find src/c/ -name *.cpp)
c_obj := $(patsubst src/c/%.cpp, build/c/%.o, $(c_src))

obj := $(c_obj) $(asm_obj)

$(c_obj): build/c/%.o : src/c/%.cpp
	mkdir -p $(dir $@) && \
	g++ -c -ffreestanding $(patsubst build/c/%.o, src/c/%.cpp, $@) -o $@
$(asm_obj): build/asm/%.o : src/asm/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/asm/%.o, src/asm/%.asm, $@) -o $@

.PHONY: build
build: $(obj)
	mkdir -p out && \
	x86_64-elf-ld -n -o out/os.bin -T target/link.ld $(obj) && \
	cp out/os.bin target/boot/os.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o out/os.iso target