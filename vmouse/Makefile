obj-m += vmouse.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)
# EXTRA_CFLAGS += -g

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules 
clean:
	@rm -fr *.ko *.o
