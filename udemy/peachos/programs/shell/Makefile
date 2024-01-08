.PHONY: all clean

FILES=./build/shell.o
INCLUDES=
FLAGS=-g -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/shell.elf
	mkdir -p ../../disk/bin
	cp ./bin/shell.elf ../../disk/bin/shell

clean:
	rm -rf build bin

./build/%.o: ./src/%.c
	mkdir -p build
	i686-elf-gcc -I./ -I../stdlib/src $(FLAGS) -std=c11 -c $< -o $@

./bin/shell.elf: $(FILES)
	mkdir -p bin
	i686-elf-gcc -g -T ./src/linker.ld -o ./bin/shell.elf -ffreestanding -O0 -nostdlib -fpic -g $(FILES) ../stdlib/bin/stdlib.elf

