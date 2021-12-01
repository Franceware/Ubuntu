#include "GestioneChat.h"

void Sender(int idQueueReicever, int idQueueSender)
{
	Message MSG;
	int error;
	
	while(1)
	{
		printf("Inserisci messaggio da inviare\n");
		scanf("%s", MSG.msg);
		
		MSG.type = MSG_TYPE;
		
		if(strcmp(MSG.msg, "exit") == 0)
		{
			error = msgsnd(idQueueReicever, (void*)&MSG, sizeof(Message) - sizeof(long), 0);
			
			if(error < 0)
			{
				perror("[SENDER] Error to Send message to Reicever\n");
				exit(1);
			}
			
			printf("[SENDER] Messaggio inviato: %s\n", MSG.msg);
			
			exit(1);
		}
		 else
		 	{
				error = msgsnd(idQueueSender, (void*)&MSG, sizeof(Message) - sizeof(long), 0);
				
				if(error < 0)
				{
					perror("[SENDER] Error to Send message to Reicever\n");
					exit(1);
				}
				
				printf("[SENDER] Messaggio inviato: %s\n", MSG.msg);	 		
		 	}	
	}
}

void Reicever(int idQueueReicever)
{
	Message MSG;
	int error;
	
	while(1)
	{
		error = msgrcv(idQueueReicever, (void*)&MSG, sizeof(Message) - sizeof(long), MSG_TYPE, 0);
		
		if(error < 0)
		{
			perror("[REICEVER] Error to Send message to Reicever\n");
			exit(1);
		}
		
		printf("[REICEVER] Messaggio ricevuto: %s\n", MSG.msg);
		
		if(strcmp(MSG.msg, "exit") == 0)	exit(1);
	}
}
