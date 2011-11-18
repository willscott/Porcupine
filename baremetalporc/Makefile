ASMFILES=porcuhello.asm.app porcubaby.asm.app porcutalk-server.asm.app porcutalk-client.asm.app
CFILES=porcuhelloc.c.app
NUMTHREADS=2
LOOPSIZE=8000

CC=gcc
AS=nasm
LD=ld
CFLAGS=-c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -finline-functions 

#Assembly
%.asm.app: %.asm bmdev.asm branches.asm
	$(AS) -f bin -o $@ $<

#C
libBareMetal.o: libBareMetal.c libBareMetal.h
	$(CC) $(CFLAGS) -o libBareMetal.o libBareMetal.c

%.c.app: %.c libBareMetal.o libBareMetal.h
	$(CC) $(CFLAGS) -o $<.o $<
	$(LD) -T app.ld -o $@ $<.o libBareMetal.o

babies:
	python ./porcumommy.py $(NUMTHREADS)
branches:
	python ./branchbuilder.py $(LOOPSIZE)

all: $(ASMFILES) $(CFILES)

install:
	echo "Copying files to BareMetal.vmdk"	
	./vmware-mount ./BareMetal.vmdk 1 ./vmdk/
	cp *.app vmdk/
	echo "Copying files to BareMetal2.vmdk"	
	./vmware-mount ./BareMetal2.vmdk 1 ./vmdk2/
	cp *.app vmdk2/
	sleep 1
	umount vmdk
	umount vmdk2

umount:
	umount vmdk
	umount vmdk2

clean:
	rm *.app
	rm *.o
	rm -rf *.lck
