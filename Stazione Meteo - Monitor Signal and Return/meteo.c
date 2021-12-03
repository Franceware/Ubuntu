#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <errno.h>

#include "StazioneMeteo.h"

#define NUM_UTENTI 10

int main()
{
	pid_t PID;
	
	key_t idMeteo = shmget(IPC_PRIVATE, sizeof(MonitorMeteo), IPC_CREAT|0664);

	MonitorMeteo* monMet = (MonitorMeteo*) shmat(idMeteo, 0, 0);

	init_monitor(&(monMet->mon), VARCONDTOTALI);
	
	monMet->numLettori = 0;
	monMet->numScrittori = 0;

	for(size_t k=0; k < NUM_UTENTI; k++)
	{
		PID = fork();

		if(PID == 0)
		{
			Utente(monMet);
			exit(0);
		}
		 else if(PID < 0)
			 perror("Fork error\n");
	}
	
	PID = fork();

	if(PID == 0)
	{
		Servizio(monMet);
		exit(0);
	}
	 else if(PID < 0)
		 perror("Fork error\n");

	for(size_t i=0; i<NUM_UTENTI + 1; i++)	wait(NULL);

	remove_monitor(&(monMet->mon));

	shmctl(idMeteo, IPC_RMID, 0);

	return 0;

}
