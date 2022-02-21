#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"


void * produttore_1(void * p)
{
	MonitorPC* pc = (MonitorPC*) p;
	
	for(int i=0; i<4; i++) 
	{
		int valore = rand() % 10;

		produci_tipo_1(pc, valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * produttore_2(void * p)
{
    MonitorPC* pc = (MonitorPC*) p;
    
	for(int i=0; i<4; i++) 
	{
		int valore = rand() % 10;
		
		produci_tipo_2(pc, valore);
		
		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore_1(void * p)
{
	MonitorPC* pc = (MonitorPC*) p;
	
	for(int i=0; i<8; i++) 
	{
		int valore;

		consuma_tipo_1(pc, &valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore_2(void * p)
{
	MonitorPC* pc = (MonitorPC*) p;
	
	for(int i=0; i<8; i++) 
	{
		int valore;
		
		consuma_tipo_2(pc, &valore);

		sleep(1);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	pthread_t thread_produttori_1[2];
	pthread_t thread_produttori_2[2];
	pthread_t thread_consumatore_1;
	pthread_t thread_consumatore_2;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
	int rc;
	int i;

	srand(time(NULL));

	MonitorPC* pc_op = malloc(sizeof(MonitorPC));
	
	inizializza(pc_op);

	for(i=0; i<2; i++) 
		pthread_create(&thread_produttori_1[i], &attr, produttore_1, (void*) pc_op);

	for(i=0; i<2; i++)
		pthread_create(&thread_produttori_2[i], &attr, produttore_2, (void*) pc_op);

	pthread_create(&thread_consumatore_1, &attr, consumatore_1, (void*) pc_op);
	
	pthread_create(&thread_consumatore_2, &attr, consumatore_2, (void*) pc_op);

	for(i=0; i<2; i++)
		pthread_join(thread_produttori_1[i], NULL);

	for(i=0; i<2; i++)
		pthread_join(thread_produttori_2[i], NULL);

	pthread_join(thread_consumatore_1, NULL);
	pthread_join(thread_consumatore_2, NULL);
	
	rimuovi(pc_op);

	free(pc_op);
}
