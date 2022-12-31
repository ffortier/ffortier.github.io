.PHONY: all clean

all: ./bin/blank.bin
	mkdir -p ../../disk/bin
	cp ./bin/blank.bin ../../disk/bin/blank

clean:
	rm -rf build bin

./build/%.asm.o: ./src/%.asm
	mkdir -p build
	nasm -f elf $< -o $@

./bin/blank.bin: ./build/blank.asm.o
	mkdir -p bin
	i686-elf-gcc -g -T ./src/linker.ld -o ./bin/blank.bin -ffreestanding -O0 -nostdlib -fpic -g ./build/blank.asm.o
