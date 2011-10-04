obj-m = porcupine.o
KDIR := /lib/modules/$(shell uname -r)/build
HERE = /home/guest/porcupine

all:
	make -C $(KDIR) M=$(HERE) modules
clean:
	make -C $(KDIR) M=$(HERE) clean
