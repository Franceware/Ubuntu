all: chat

chat: main.o GestioneChat.o
	gcc -g -o chat main.o GestioneChat.o

main.o: main.c GestioneChat.h
	gcc -g -c main.c

GestioneChat.o: GestioneChat.c GestioneChat.h
	gcc -g -c GestioneChat.c

clean:
	rm -f *.o
	rm -f ./chat
	rm -f *~
