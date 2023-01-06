.PHONY: all run clean test debug _fat16

TEST_FILES = ./build/test/fs/pparser_test
ASM_FILES = $(shell find src -name '*.asm' -not -wholename 'src/boot/boot.asm')
TEST_C_FILES = $(shell find src -name '*_test.c')
C_FILES = $(shell find src -name '*.c' -not -name '*_test.c')
FILES = $(ASM_FILES:src/%=build/%.o) $(C_FILES:src/%.c=build/%.o)
TEST_FILES = $(TEST_C_FILES:src/%.c=build/%)
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc
EMPTY :=
SPACE := $(EMPTY) $(EMPTY)

all:
	$(MAKE) -C programs/stdlib all
	$(MAKE) -C programs/blank all
	$(MAKE) ./bin/os.bin

_fat16:
	@mkdir -p /mnt/d
	mount -t vfat ./bin/vfat16.bin /mnt/d
	cp -R disk/* /mnt/d
	umount /mnt/d

run: all
	qemu-system-i386 -no-reboot -hda bin/os.bin

test: $(TEST_FILES) ./bin/vfat16.bin
	$(subst $(SPACE), && ,$(TEST_FILES))

debug: all
	gdb-multiarch -x debug.gdb

./bin/vfat16.bin: ./bin/boot.bin $(shell find disk/*)
	dd if=/dev/zero of=./bin/vfat16.bin bs=1048576 count=16
	dd if=./bin/boot.bin of=./bin/vfat16.bin conv=notrunc
	sudo $(MAKE) _fat16

./bin/os.bin: ./bin/vfat16.bin ./bin/kernel.bin
	cp ./bin/vfat16.bin ./bin/os.bin
	dd if=./bin/kernel.bin of=./bin/os.bin conv=notrunc seek=512 bs=1

./bin/kernel.bin: $(FILES)
	@mkdir -p $(@D)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc $(FLAGS) -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	@mkdir -p $(@D)
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/%.asm.o: ./src/%.asm
	@mkdir -p $(@D)
	nasm -f elf -g $< -o $@

./build/%.o: ./src/%.c
	@mkdir -p $(@D)
	i686-elf-gcc $(INCLUDES) -I$(@D) $(FLAGS) -std=gnu99 -c $< -o $@

clean:
	rm -rf bin build
	$(MAKE) -C programs/blank clean
	$(MAKE) -C programs/stdlib clean

./build/%_test: ./src/%_test.c
	@mkdir -p $(@D)
	gcc -g -Dtesting -DNAMESPACE=PEACHOS $(INCLUDES) -I$(@D) -std=gnu99 -o $@ $<
	chmod +x $@
