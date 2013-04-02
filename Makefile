
CC=gcc
CFLAGS=-std=c99 -pedantic -Wall
LDFLAGS=-lm

all: stego

tiff.o: tiff.c tiff.h
	$(CC) -c $(CFLAGS) tiff.c -o tiff.o

util.o: util.h util.c
	$(CC) -c $(CFLAGS) util.c -o util.o

stego.o: stego.c stego.h
	$(CC) -c $(CFLAGS) stego.c -o stego.o

image_stego.o: image_stego.c
	$(CC) -c $(CFLAGS) image_stego.c -o image_stego.o

stego: stego.o util.o tiff.o bmp.o image_stego.o
	$(CC) $(CFLAGS) image_stego.o stego.o util.o tiff.o bmp.o $(LDFLAGS) -o stego

bmp.o: bmp.c bmp.h
	$(CC) -c $(CFLAGS) bmp.c -o bmp.o

clean:
	rm -f *.exe *.o *.stackdump stego

srcbundle:
	rm -f image_stego-src.tar.gz
	tar -czf image_stego-src.tar.gz *.h *.c Makefile README
