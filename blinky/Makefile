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

build:
	$(GCC) -mcpu=cortex-m3 -mthumb -Os -ffreestanding -nostdlib -nostartfiles -fno-builtin -Wl,-T,link.f1.ld -Wl,--gc-sections blinky.f1.c -o blinky.elf
	$(OCPY) -O binary blinky.elf blinky.bin

flash:
	$(FLASH) write blinky.bin 0x08000000
	$(FLASH) reset

flash_catch:
	$(FLASH) write catch.bin 0x08000000
	$(FLASH) reset

patch:
	$(PYTHON) patch.py

dump:
	$(FLASH) read dump.bin 0x08000000 256

showdump:
	$(HEXL) dump.bin

showcatch:
	$(HEXL) catch.bin

lock:
	$(OCD) -f stlink.cfg -f target.cfg -c "init" -c "halt" -c "stm32f1x lock 0" -c "shutdown"

unlock:
	$(OCD) -f stlink.cfg -f target.cfg -c "init" -c "halt" -c "stm32f1x unlock 0" -c "shutdown"

diff:
	$(HEXL) blinky.bin
	$(HEXL) dump.bin

decode:
	$(TR) -d '\r' < catch.txt | $(XXD) -r -p > catch.bin

disassemble:
	$(ODMP) \
		-b binary \
		-m arm \
		-M force-thumb \
		-D \
		--adjust-vma=0x08000000 \
		--start-address=0x08000008 \
		--stop-address=0x0800004c \
		catch.bin > catch.asm
