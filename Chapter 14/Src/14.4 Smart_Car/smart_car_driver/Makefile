ifneq ($(KERNELRELEASE),)
obj-m := mydriver.o
else
KERNEL_DIR := /usr/Digilent-linux-kernel
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules ARCH=arm
clean:
	rm *.o *.ko *.mod.c
endif
