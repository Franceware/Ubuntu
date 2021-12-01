all: program_ex

program_ex: main.o Collettore.o Sensore.o Aggregatore.o
	gcc -o program_ex -pthread main.o Collettore.o Sensore.o Aggregatore.o -lpthread

main.o: main.c Sensore.h
	gcc -c main.c -o main.o

Aggregatore.o: Aggregatore.c Aggregatore.h Sensore.h
	gcc -c Aggregatore.c -o Aggregatore.o -lpthread

Collettore.o: Collettore.c Collettore.h Sensore.h
	gcc -c Collettore.c -o Collettore.o 

Sensore.o: Sensore.c Sensore.h
	gcc -c Sensore.c -o Sensore.o

clean:
	rm -f *.o
	rm -f program_ex
