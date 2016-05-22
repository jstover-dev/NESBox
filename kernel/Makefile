obj-m += nesdev.o

#ccflags-y += -Wall -Werror

KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
