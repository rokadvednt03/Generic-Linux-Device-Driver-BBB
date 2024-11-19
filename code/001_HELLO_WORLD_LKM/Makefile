obj-m := 003_BBB.o
ARCH=arm
CROSS_COMPILE=arm-linux-gnueabihf-
KERN=/lib/modules/6.8.0-rc7/build/
HOST_KERN=/lib/modules/$(shell uname -r)/build/

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN) M=$(PWD) modules
clean: 
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN) M=$(PWD) clean
help:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KERN) M=$(PWD) help
host: 
	make -C $(HOST_KERN) M=$(PWD) modules



