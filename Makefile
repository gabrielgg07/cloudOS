# === Variables ===
ASM        := nasm
ASMFLAGS   := -f elf32
BOOT_SRC   := boot/boot.s
BOOT_OBJ   := build/boot.o

TARGET     := kernel.bin
ISO        := build/os.iso
GRUB_CFG   := boot/grub.cfg
LINKER     := boot/linker.ld
C_SOURCES  := $(wildcard kernel/*.c) $(wildcard kernel/drivers/*.c) $(wildcard kernel/include/*.c)
OBJ_FILES := $(patsubst %.c, %.o, $(C_SOURCES))


CC         := gcc
LD         := ld -m elf_i386
CFLAGS     := -ffreestanding -m32 -O2 -Wall -Wextra
LDFLAGS    := -T $(LINKER) -nostdlib

# === Default Build ===
all: $(ISO)

# === Link Kernel ===
build/$(TARGET): $(BOOT_OBJ) $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^


# === Compile C source files ===
build/%.o: kernel/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

# === Compile assembly file === 
$(BOOT_OBJ): $(BOOT_SRC)
	mkdir -p build
	$(ASM) $(ASMFLAGS) $< -o $@


# === ISO Image with GRUB ===
$(ISO): build/$(TARGET) $(GRUB_CFG)
	mkdir -p build/iso/boot/grub
	cp build/$(TARGET) build/iso/boot/kernel.bin
	cp $(GRUB_CFG) build/iso/boot/grub/
	grub-mkrescue -o $(ISO) build/iso

# === Run in QEMU ===
run: $(ISO)
	qemu-system-x86_64 -cdrom $(ISO)



# === Clean Build ===
clean:
	rm -rf build
