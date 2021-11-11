CC=gcc -Wall -Og -g

project2: main.o
	$(CC) -o project4 main.o

main.o:	main.c headers.h
	$(CC) -c main.c

clean:
	rm *.o -f
	touch *.c
	rm project4 -f
