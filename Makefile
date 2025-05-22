CFLAGS = -ffreestanding -nostdlib -mcmodel=medany

build: clean
	mkdir -p build
	riscv64-unknown-elf-gcc -T kernel.lds $(CFLAGS) -o build/kernel *.S *.c -lgcc

qemu: build
	unset GTK_PATH && \
	qemu-system-riscv64 -M virt -m 4G -kernel build/kernel -serial stdio -device ramfb

clean:
	rm -rf build
