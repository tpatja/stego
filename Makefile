
CFLAGS=-std=c99 -pedantic -Wall
LDFLAGS=-lm

all: stego

tiff.o: tiff.c tiff.h
	gcc -c $(CFLAGS) tiff.c -o tiff.o

util.o: util.h util.c
	gcc -c $(CFLAGS) util.c -o util.o

stego.o: stego.c stego.h
	gcc -c $(CFLAGS) stego.c -o stego.o

image_stego.o: image_stego.c
	gcc -c $(CFLAGS) image_stego.c -o image_stego.o

stego: stego.o util.o tiff.o bmp.o image_stego.o
	gcc $(CFLAGS) image_stego.o stego.o util.o tiff.o bmp.o $(LDFLAGS) -o stego

bmp.o: bmp.c bmp.h
	gcc -c $(CFLAGS) bmp.c -o bmp.o

clean:
	rm -f *.exe *.o *.stackdump stego

srcbundle:
	rm -f image_stego-src.tar.gz
	tar -czf image_stego-src.tar.gz *.h *.c Makefile README
