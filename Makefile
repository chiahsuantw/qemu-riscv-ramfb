CFLAGS = -nostdlib -ffreestanding -mcmodel=medany

build: clean
	mkdir -p build
	riscv64-unknown-elf-gcc -T kernel.ld $(CFLAGS) -o build/kernel *.S *.c

qemu: build
	qemu-system-riscv64 -M virt -kernel build/kernel -serial stdio -device ramfb

clean:
	rm -rf build
