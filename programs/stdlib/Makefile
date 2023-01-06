.PHONY: all clean

FILES=./build/start.asm.o ./build/peachos.asm.o ./build/stdlib.o
INCLUDES=./src
FLAGS=-g

all: ./bin/stdlib.elf

clean:
	rm -rf build bin

./build/%.asm.o: ./src/%.asm
	mkdir -p build
	nasm -f elf $< -o $@

./build/%.o: ./src/%.c
	mkdir -p build
	i686-elf-gcc -I$(INCLUDES) $(FLAGS) -std=gnu99 -c $< -o $@

./bin/stdlib.elf: $(FILES)
	mkdir -p bin
	i686-elf-ld -m elf_i386 -relocatable $(FILES) -o ./bin/stdlib.elf

