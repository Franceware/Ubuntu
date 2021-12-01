#include "Sensore.h"
#include "Aggregatore.h"
#include "Collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>

int main()
{
	pid_t pid, pidL;
	int idQueueSensor, idQueueCollector[NUM_COLLETTORI];

	idQueueSensor = msgget(IPC_PRIVATE, IPC_CREAT|0664);

	for(size_t i=0; i<NUM_COLLETTORI; i++)	idQueueCollector[i] = msgget(IPC_PRIVATE, IPC_CREAT|0664);

	pid = fork();

	if(pid == 0)
	{
		generaLettura(idQueueSensor);
		exit(2);
	}

	pidL = fork();
	
	if(pidL == 0)
	{
		collettore(idQueueCollector[0]);
		exit(2);
	}
	
	pidL = fork();
	
	if(pidL == 0)
	{
		collettore(idQueueCollector[1]);
		exit(2);
	}
	
	pidL = fork();
	
	if(pidL == 0)
	{
		collettore(idQueueCollector[2]);
		exit(2);
	}
	
	pid = fork();

	if(pid == 0)
	{
		aggregazione(idQueueSensor, idQueueCollector);
		exit(2);
	}
	
	for(size_t i=0; i<5; i++)	wait(NULL);
	
	msgctl(idQueueSensor, IPC_RMID, NULL);
	msgctl(idQueueCollector[0], IPC_RMID, NULL);
	msgctl(idQueueCollector[1], IPC_RMID, NULL);
	msgctl(idQueueCollector[2], IPC_RMID, NULL);	
	
	return 0;

}

