#include "Sensore.h"
#include "Collettore.h"

void collettore(int idQueueCollettore)
{
	Messaggio MSG;
	int error;
	
	printf("[COLLETTORE] Avvio processo collettore (idQueue = %d)...\n", idQueueCollettore);

	for(size_t i=0; i<NUM_GENERAZIONI; i++)
	{
		printf("[COLLETTORE] In attesa di messaggi...\n");

		error = msgrcv(idQueueCollettore, (void*)&MSG, sizeof(Messaggio)-sizeof(long), MSG_TYPE, 0);
		
		if(error < 0)
		{
			perror("[COLLETTORE] Error from REICEVE\n");
			exit(1);
		}

		int val = MSG.lettura; 
		printf("[COLLETTORE] <%lu> Collettore: Ricevuto valore=%d\n", i, val);
	}
}
