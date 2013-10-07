ifneq ($(KERNELRELEASE),)
obj-m := pwm_driver.o
else
KERNEL_DIR := <YOUR_DIR>/ZedBoard/Kernel/Digilent-linux-3.3-digilent-7f8e908
PWD := $(shell pwd)
all:
	$(MAKE) -C $(KERNEL_DIR) SUBDIRS=$(PWD) modules ARCH=arm
clean:
	rm *.o *.ko *.mod.c
endif
