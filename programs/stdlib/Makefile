.PHONY: all clean test

C_SRCS=$(wildcard src/*.c)
ASM_SRCS=$(wildcard src/*.asm)
FILES=$(ASM_SRCS:src/%=build/%.o) $(C_SRCS:src/%.c=build/%.o)
INCLUDES=./src
FLAGS = -g -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/stdlib.elf

clean:
	rm -rf build bin

test: ./build/stdlib_test
	./build/stdlib_test

./build/%.asm.o: ./src/%.asm
	mkdir -p build
	nasm -f elf $< -o $@

./build/%.o: ./src/%.c
	mkdir -p build
	i686-elf-gcc -I$(INCLUDES) $(FLAGS) -std=c11 -c $< -o $@

./build/%_test: ./tests/%_test.c
	mkdir -p build
	gcc -Dtesting -g -I$(INCLUDES) -I../../src -std=c11 -o $@ $<
	chmod +x $@

./bin/stdlib.elf: $(FILES)
	mkdir -p bin
	i686-elf-ld -m elf_i386 -relocatable $(FILES) -o ./bin/stdlib.elf

