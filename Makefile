# === Variables ===
ASM        := nasm
ASMFLAGS   := -f elf32
CC         := gcc
LD         := ld -m elf_i386

CFLAGS     := -ffreestanding -m32 -O2 -Wall -Wextra -Iinclude
LDFLAGS    := -T boot/linker.ld -nostdlib

BOOT_SRC   := boot/boot.s
BOOT_OBJ   := build/boot.o

TARGET     := kernel.bin
ISO        := build/os.iso
GRUB_CFG   := boot/grub.cfg

# === Source Files ===
C_SRC_DIRS := kernel kernel/drivers kernel/include kernel/arch/x86
C_SOURCES  := $(foreach dir,$(C_SRC_DIRS),$(wildcard $(dir)/*.c))
ASM_SOURCES := $(wildcard $(foreach dir,$(C_SRC_DIRS),$(dir)/*.s))
OBJ_FILES  := $(patsubst %.c, build/%.o, $(notdir $(C_SOURCES))) \
              $(patsubst %.s, build/%.o, $(notdir $(ASM_SOURCES)))

# === Default Build ===
all: $(ISO)

# === Link Kernel ===
build/$(TARGET): $(BOOT_OBJ) $(OBJ_FILES)
	$(LD) $(LDFLAGS) -o $@ $^

# === Compile C source files ===
# Search each directory for the matching file name
build/%.o:
	mkdir -p build
	@src_c=$(firstword $(wildcard $(addsuffix /$*.c,$(C_SRC_DIRS)))) && \
	if [ -n "$$src_c" ]; then \
		$(CC) $(CFLAGS) -c $$src_c -o $@ ; \
	else \
		src_s=$(firstword $(wildcard $(addsuffix /$*.s,$(C_SRC_DIRS)))) && \
		$(ASM) $(ASMFLAGS) $$src_s -o $@ ; \
	fi

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
