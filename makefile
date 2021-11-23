CC=gcc -Wall -Og -g

project2: main.o read.o rewrite.o
	$(CC) -o project4 main.o read.o rewrite.o

main.o:	main.c headers.h
	$(CC) -c main.c

read.o:	read.c headers.h
	$(CC) -c read.c

rewrite.o:	rewrite.c headers.h
	$(CC) -c rewrite.c

clean:
	rm *.o -f
	touch *.c
	rm project4 -f
