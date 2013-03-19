
CFLAGS=-std=c99 -pedantic -Wall

tifftest: tiff.c util.o
	gcc $(CFLAGS) tiff.c util.o -o tifftest

util.o: util.h util.c
	gcc -c $(CFLAGS) util.c -o util.o

stego: stego.c util.o
	gcc $(CFLAGS) stego.c util.o -o stego


all: tifftest util.o

clean:
	rm -f tifftest.exe

test: tifftest
	./tifftest.exe ../nokia.tif foo.tif
	./tifftest.exe ../nokia2.tif foo2.tif
	./tifftest.exe ../nokia_grayscale.tif foo_grayscale.tif
#../cicrles.tif
stegotest: stego
	./stego.exe
