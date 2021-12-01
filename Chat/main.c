#include "GestioneChat.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

int main(int argc, char* argv[])
{
	char firstChar, secondChar;
	pid_t pid;
	key_t queueSender, queueReicever;
	int idQueueSender, idQueueReicever;
	
	
	if(argc < 2)
	{
		printf("[MAIN] Error: Fornire come parametri di ingresso 2 caratteri separati da spazio\n");
        printf("Esempio: %s a b\n", argv[0]);
		_exit(1);
	}
	
	firstChar = *argv[1];
	secondChar = *argv[2];
	
	printf("I caratteri inseriti sono: %c %c\n", firstChar, secondChar);
	
	queueSender = ftok(FTOK_PATH, firstChar);
	queueReicever = ftok(FTOK_PATH, secondChar);

	idQueueSender = msgget(queueSender, IPC_CREAT|0644);

	if(idQueueSender < 0) {
		perror("[MAIN] Msgget fallita");
		exit(1);
	}

    idQueueReicever = msgget(queueReicever, IPC_CREAT|0644);

	if(idQueueReicever < 0) {
		perror("[MAIN] Msgget fallita");
		exit(1);
	}

	for(size_t i=0; i<2; i++)
	{
		pid = fork();
		
		if(pid==0)
		{	
			if(i==0)	Sender(idQueueReicever, idQueueSender);
			 else	Reicever(idQueueReicever);
		}
		 else if (pid < 0)
		 	  {
				perror("Fork fallita");
				exit(1);
		      }	
	}

	for(size_t i=0; i<2; i++) 	wait(NULL);

	msgctl(idQueueSender, IPC_RMID, 0);
	msgctl(idQueueReicever, IPC_RMID, 0);
}
