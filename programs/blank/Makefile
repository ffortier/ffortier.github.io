.PHONY: all clean

FILES=./build/blank.o
INCLUDES=
FLAGS=-g

all: ./bin/blank.elf
	mkdir -p ../../disk/bin
	cp ./bin/blank.elf ../../disk/bin/blank

clean:
	rm -rf build bin

./build/%.asm.o: ./src/%.asm
	mkdir -p build
	nasm -f elf $< -o $@

./build/%.o: ./src/%.c
	mkdir -p build
	i686-elf-gcc -I./ -I../stdlib/src $(FLAGS) -std=gnu99 -c $< -o $@

./bin/blank.elf: $(FILES)
	mkdir -p bin
	i686-elf-gcc -g -T ./src/linker.ld -o ./bin/blank.elf -ffreestanding -O0 -nostdlib -fpic -g $(FILES) ../stdlib/bin/stdlib.elf

