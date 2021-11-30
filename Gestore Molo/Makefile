all: main_ex

main_ex: main.o Molo.o
	gcc -o main_ex main.o Molo.o -lpthread

Treno.o: Molo.c Molo.h
	gcc -c Molo.c

main.o: main.c Molo.h
	gcc -c main.c

clean:
	rm -f *.o
	rm -f ./main_ex
