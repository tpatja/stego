
CFLAGS=-std=c99 -pedantic -Wall

tifftest: tiff.c
	gcc $(CFLAGS) tiff.c -o tifftest

all: tifftest

clean:
	rm -f tifftest.exe

test: tifftest
	./tifftest.exe ../nokia.tif foo.tif
#../cicrles.tif
