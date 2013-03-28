
CFLAGS=-std=c99 -pedantic -Wall

all: stego tiff_test

tiff_test: tiff_test.c tiff.o util.o
	gcc $(CFLAGS) tiff_test.c tiff.o util.o -o tiff_test

tiff.o: tiff.c tiff.h
	gcc -c $(CFLAGS) tiff.c -o tiff.o

util.o: util.h util.c
	gcc -c $(CFLAGS) util.c -o util.o

stego.o: stego.c stego.h
	gcc -c $(CFLAGS) stego.c -o stego.o

image_stego.o: image_stego.c
	gcc -c $(CFLAGS) image_stego.c -o image_stego.o

stego: stego.o util.o tiff.o bmp.o image_stego.o
	gcc $(CFLAGS) image_stego.o stego.o util.o tiff.o bmp.o -o stego


bmp.o: bmp.c bmp.h

bmp_test: bmp.o util.o bmp_test.c
	gcc $(CFLAGS) bmp.o util.o bmp_test.c -o bmp_test


clean:
	rm -f *.exe *.o *.stackdump

test_tiff: tiff_test
	./tiff_test ./img/nokia.tif ./img/foo.tif
	./tiff_test ./img/nokia2.tif ./img/foo2.tif
	./tiff_test ./img/nokia_grayscale.tif ./img/foo_grayscale.tif

test_stego: stego
	./stego


srcbundle:
	rm -f image_stego-src.tar.gz
	tar -czf image_stego-src.tar.gz *.h *.c Makefile
