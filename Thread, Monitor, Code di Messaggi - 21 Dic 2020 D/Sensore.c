#include "Sensore.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>

#define NUM_MAX_GENERATO 10

void generaLettura(int idQueueSensor)
{
	int valGenerato, error;
	Messaggio MSG;
	
	printf("[SENSORE] Avvio Process Sensore...\n");

	srand(time(NULL));

	for(size_t i=0; i<NUM_GENERAZIONI; i++)
	{
		valGenerato = rand()% NUM_MAX_GENERATO + 1;

		printf("[SENSORE] Invio valore --> %d\n", valGenerato);
		MSG.type = MSG_TYPE;
		MSG.lettura = valGenerato;

		error = msgsnd(idQueueSensor, (void*)&MSG, sizeof(Messaggio) - sizeof(long), 0);
		
		if(error < 0)
		{
			perror("[SENSORE] Error Send to Queue\n");
			exit(1);
		}
		 else
			 printf("[SENSOR] Messaggio n°%lu inviato correttamente\n", i);

		sleep(1);
	}
}
