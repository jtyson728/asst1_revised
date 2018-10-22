CC = gcc
CFLAGS = -Wall -Werror -g -lm -fsanitize=address

all: scannerCSVsorter

scannerCSVsorter: scannerCSVsorter.c scannerCSVsorter.h
	$(CC) $(CFLAGS) -o scannerCSVsorter scannerCSVsorter.c mergesort.c

scannerCSVsorter.o: scannerCSVsorter.c scannerCSVsorter.h
	$(CC) -c scannerCSVsorter.c

mergesort.o: mergesort.c mergesort.h
	$(CC) -c mergesort.c

clean:
	rm -f scannerCSVsorter *.o
