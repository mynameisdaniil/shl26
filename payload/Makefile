GCC = /usr/bin/env arm-none-eabi-gcc
OCPY = /usr/bin/env arm-none-eabi-objcopy

build:
	$(GCC) -mcpu=cortex-m3 -mthumb -Os -nostartfiles -Wl,-T,ram.ld entry.S payload.c -o payload.elf
	$(OCPY) -O binary payload.elf payload.bin
