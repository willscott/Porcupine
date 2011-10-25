ASMFILES=porchello.asm.app
CFILES=porchelloc.c.app

CC=gcc
AS=nasm
LD=ld
CFLAGS=-c -m64 -nostdlib -nostartfiles -nodefaultlibs -fomit-frame-pointer -finline-functions 

#Assembly
%.asm.app: %.asm bmdev.asm
	$(AS) -f bin -o $@ $<

#C
libBareMetal.o: libBareMetal.c libBareMetal.h
	$(CC) $(CFLAGS) -o libBareMetal.o libBareMetal.c

%.c.app: %.c libBareMetal.o libBareMetal.h
	$(CC) $(CFLAGS) -o $<.o $<
	$(LD) -T app.ld -o $@ $<.o libBareMetal.o

all: $(ASMFILES) $(CFILES)

install:
	echo "Copying files to BareMetal.vmdk"	
	./vmware-mount ./BareMetal.vmdk 1 ./vmdk/
	cp *.app vmdk/
	umount vmdk

clean:
	rm *.app
	rm *.o
