# === Variables ===
ASM        := nasm
ASMFLAGS   := -f elf64
BOOT_SRC   := boot/boot.s
BOOT_OBJ   := build/boot.o

TARGET     := kernel.bin
ISO        := build/os.iso
GRUB_CFG   := boot/grub.cfg
LINKER     := boot/linker.ld
C_SOURCES  := $(wildcard kernel/*.c)
OBJ_FILES  := $(patsubst kernel/%.c, build/%.o, $(C_SOURCES))

CC         := gcc
LD         := ld
CFLAGS     := -ffreestanding -m64 -O2 -Wall -Wextra
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
run: build/$(TARGET)
	qemu-system-x86_64 -kernel build/$(TARGET) -nographic -serial mon:stdio


# === Clean Build ===
clean:
	rm -rf build
