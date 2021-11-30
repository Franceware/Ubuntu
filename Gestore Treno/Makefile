all: main_ex

main_ex: main.o Treno.o
	gcc -o main main.o Treno.o -lpthread

Treno.o: Treno.c Treno.h
	gcc -c Treno.c

main.o: main.c Treno.h
	gcc -c main.c

clean:
	rm -f *.o
	rm -f ./start
