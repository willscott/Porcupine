obj-m = porcupine.o
KDIR := /lib/modules/$(shell uname -r)/build
HERE = $(shell pwd)
NUMTHREADS = 2

all:
	python porcumommy.py $(NUMTHREADS)
	make -C $(KDIR) M=$(HERE) modules
clean:
	make -C $(KDIR) M=$(HERE) clean
