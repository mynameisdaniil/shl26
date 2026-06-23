GCC = /usr/bin/env arm-none-eabi-gcc
OCPY = /usr/bin/env arm-none-eabi-objcopy
ODMP = /usr/bin/env arm-none-eabi-objdump
FLASH = /usr/bin/env st-flash
OCD = /usr/bin/env openocd
HEXL = /usr/bin/env hexyl
SCR = /usr/bin/env screen
TR = /usr/bin/env tr
XXD = /usr/bin/env xxd
PYTHON = /usr/bin/env python3

.PHONY: build payload flash flash_catch patch dump upload_payload lock unlock diff decode clean

build:
	$(GCC) -mcpu=cortex-m3 -mthumb -Os -ffreestanding -nostdlib -nostartfiles -fno-builtin -Wl,-T,blinky/link.f1.ld -Wl,--gc-sections blinky/blinky.f1.c -o blinky/blinky.elf
	$(OCPY) -O binary blinky/blinky.elf blinky/blinky.bin

payload:
	$(GCC) -mcpu=cortex-m3 -mthumb -Os -nostartfiles -Wl,-T,payload/ram.ld payload/entry.S payload/payload.c -o payload/payload.elf
	$(OCPY) -O binary payload/payload.elf payload/payload.bin

flash:
	$(FLASH) write ./blinky/blinky.bin 0x08000000
	$(FLASH) reset

flash_catch:
	$(FLASH) write catch.bin 0x08000000
	$(FLASH) reset

patch:
	$(PYTHON) patch.py

dump:
	$(FLASH) read dump.bin 0x08000000 256

upload_payload:
	$(OCD) -f stlink.cfg -f target.cfg -c "init" -c "load_image \"./payload/payload.bin\" 0x20000000" -c "exit"

lock:
	$(OCD) -f stlink.cfg -f target.cfg -c "init" -c "halt" -c "stm32f1x lock 0" -c "shutdown"

unlock:
	$(OCD) -f stlink.cfg -f target.cfg -c "init" -c "halt" -c "stm32f1x unlock 0" -c "shutdown"

diff:
	$(HEXL) blinky/blinky.bin
	$(HEXL) dump.bin

decode:
	$(TR) -d '\r' < catch.txt | $(XXD) -r -p > catch.bin

clean:
	rm -f catch.* blinky/*.bin blinky/*.o blinky/*.elf payload/*.bin payload/*.o payload/*.elf
