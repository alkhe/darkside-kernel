i386	?= i586-elf
ARM	?= arm-none-eabi
MOUNT ?= /Volumes/RASPI
EJECT ?= sudo eject

OS = $(shell uname -s)

CFLAGS_i386	= -O -fno-asynchronous-unwind-tables -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin
CFLAGS_RASPI	= -O -fno-asynchronous-unwind-tables -fstrength-reduce -fomit-frame-pointer -finline-functions -Wall -O6 -nostdinc -ffreestanding -nostartfiles -nodefaultlibs -marm -mcpu=arm1176jzf-s

.PHONY: kernel-i386.elf kernel-raspi.elf kernel.img kernel-raspi.img

all:

kernel-i386.elf:
	cd src/hal/i386; make
	cd src/kernel; make ARCH="i386" BLD_TARGET="i586-elf" CFLAGS="$(CFLAGS_i386)"
	cd src/drivers/graphics/vga; make
	cd src/drivers/ps2; make
	cd src/lib/libgeneric; make ARCH="i386" BLD_TARGET="i586-elf" CFLAGS="$(CFLAGS_i386)"
	$(i386)-ld -T linker.ld -o $@ build-i386/hal/i386/asm/*.o build-i386/hal/i386/*.o build-i386/kernel/*.o build-i386/drivers/graphics/vga/*.o build-i386/drivers/ps2/*.o -Llib -lgeneric-i386

kernel-raspi.elf:
	cd src/hal/raspi; make CC=$(ARM)
	cd src/drivers/graphics/raspi; make CC=$(ARM)
	cd src/drivers/raspi; make CC=$(ARM)
#	cd src/kernel; make CC=$(ARM) BLD_TARGET=raspi CFLAGS=$(CFLAGS_RASPI)
	cd src/lib/libgeneric; make CC=$(ARM) BLD_TARGET=raspi CFLAGS="$(CFLAGS_RASPI)"
	$(ARM)-ld -T linker.pi -o $@ lib/raspi-libgcc.a build-raspi/hal/raspi/*.o build-raspi/drivers/graphics/raspi/*.o build-raspi/drivers/raspi/*/*.o lib/libgeneric-raspi.a

kernel-raspi.img: kernel-raspi.elf
	$(ARM)-objcopy kernel-raspi.elf -O binary kernel-raspi.img

raspi-install:
	cp kernel-raspi.img $(MOUNT)/kernel.img
	$(EJECT) $(MOUNT)

i386-osx: kernel-i386.elf
	sudo diskutil attach dsos-fat-i386.img
	sudo cp kernel-i386.elf /Volumes/Untitled/boot
	sudo cp menu.lst /Volumes/Untitled/boot/grub
	sudo diskutil eject Untitled

i386-linux: kernel-i386.elf
	sudo losetup -o32256 /dev/loop0 dsos-fat-i386.img
	sudo mount /dev/loop0 /mnt/hdd
	sudo cp kernel-i386.elf /mnt/hdd/boot
	sudo cp menu.lst /mnt/hdd/boot/grub
	sudo umount /dev/loop0
	sudo losetup -d /dev/loop0

i386: kernel-i386.elf
	./updateimage.sh