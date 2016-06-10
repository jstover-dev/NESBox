obj-m += nesbox.o

#ccflags-y += -Wall -Werror -fdiagnostics-color

KDIR := /lib/modules/$(shell uname -r)/build

build:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
