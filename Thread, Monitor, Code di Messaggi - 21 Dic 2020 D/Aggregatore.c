#include "Sensore.h"
#include "Aggregatore.h"

void aggregazione(int idQueueSensore, int* idQueueCollettori)
{
	pthread_t Scrittore;
	pthread_t Lettori[NUM_LETTORI];
	
	printf("[AGGREGATORE] Avvio Process Aggregatore...\n");
	
	MonitorAggregatore* Agg = (MonitorAggregatore*) malloc(sizeof(MonitorAggregatore));

	pthread_mutex_init(&(Agg->Mutex), NULL);
	pthread_cond_init(&(Agg->CondVarLettori), NULL);
	pthread_cond_init(&(Agg->CondVarScrittori), NULL);

	Agg->codaSensori = idQueueSensore;
	Agg->codaCollettori[0] = idQueueCollettori[0];
	Agg->codaCollettori[1] = idQueueCollettori[1];
	Agg->codaCollettori[2] = idQueueCollettori[2];
	
	Agg->numLettori = Agg->numScrittori = 0;
	Agg->waitLettori = Agg->waitScrittori = 0;
	
	pthread_create(&Scrittore, NULL, threadScrittore, (void*)Agg);
	pthread_create(&Lettori[0], NULL, threadLettore1, (void*)Agg);
	pthread_create(&Lettori[1], NULL, threadLettore2, (void*)Agg);
	pthread_create(&Lettori[2], NULL, threadLettore3, (void*)Agg);
	
	for(size_t i=0; i<NUM_LETTORI; i++)	pthread_join(Lettori[i], NULL);
	
	pthread_join(Scrittore, NULL);
	
	pthread_mutex_destroy(&(Agg->Mutex));
	pthread_cond_destroy(&(Agg->CondVarLettori));
	pthread_cond_destroy(&(Agg->CondVarScrittori));
}

void * threadScrittore(void * Agg)
{
	int error;
	MonitorAggregatore* Monitor = (MonitorAggregatore*) Agg;
	Messaggio MSG;
	
	for(size_t i=0; i<NUM_GENERAZIONI; i++)
	{
		printf("[THREAD SCRITTORE] Aggregatore: In attesa di messaggi...\n");
		
		error = msgrcv(Monitor->codaSensori, (void*)&MSG, sizeof(Messaggio)-sizeof(long), MSG_TYPE, 0);
		
		if(error < 0)
		{
			perror("[THREAD SCRITTORE] Error from REICEVE codaSensore\n");
			exit(1);
		}
		
		int val = MSG.lettura;
		
		printf("[THREAD SCRITTORE] Aggregatore: Ricevuto valore = %d\n", val);
		
		scrittura(Monitor, val);
	}
	
	pthread_exit(NULL);
}

void * threadLettore1(void * Agg)
{
	int val, error;
	MonitorAggregatore* Monitor = (MonitorAggregatore*) Agg;
	Messaggio MSG;
	
	for(size_t i=0; i<NUM_GENERAZIONI; i++)
	{
		sleep(1);
		
		lettura(Monitor, &val);
		
		printf("[THREAD LETTORE 1]Aggregatore: Invio valore=%d\n", val);
	
		MSG.type = MSG_TYPE;
		MSG.lettura = val;
		
		error = msgsnd(Monitor->codaCollettori[0], (void*)&MSG, sizeof(Messaggio)-sizeof(long), 0);
		
		if(error < 0)
		{
			perror("[THREAD LETTORE 1] Error to SEND codaCollettori[0]\n");
			exit(1);
		}
		 else
		 	printf("[THREAD LETTORE 1] <%lu> SEND to codaCollettori[0] riuscita\n", i);
	}
	
	pthread_exit(NULL);
}

void * threadLettore2(void * Agg)
{
	int val, error;
	MonitorAggregatore* Monitor = (MonitorAggregatore*) Agg;
	Messaggio MSG;
	
	for(size_t i=0; i<NUM_GENERAZIONI; i++)
	{
		sleep(1);
		
		lettura(Monitor, &val);
		
		printf("[THREAD LETTORE 2]Aggregatore: Invio valore=%d\n", val);
	
		MSG.type = MSG_TYPE;
		MSG.lettura = val;
		
		error = msgsnd(Monitor->codaCollettori[1], (void*)&MSG, sizeof(Messaggio)-sizeof(long), 0);
		
		if(error < 0)
		{
			perror("[THREAD LETTORE 2] Error to SEND codaCollettori[1]\n");
			exit(1);
		}
		 else
		 	printf("[THREAD LETTORE 2] <%lu> SEND to codaCollettori[1] riuscita\n", i);
	}
	
	pthread_exit(NULL);
}

void * threadLettore3(void * Agg)
{
	int val, error;
	MonitorAggregatore* Monitor = (MonitorAggregatore*) Agg;
	Messaggio MSG;
	
	for(size_t i=0; i<NUM_GENERAZIONI; i++)
	{
		sleep(1);
		
		lettura(Monitor, &val);
		
		printf("[THREAD LETTORE 3]Aggregatore: Invio valore=%d\n", val);
	
		MSG.type = MSG_TYPE;
		MSG.lettura = val;
		
		error = msgsnd(Monitor->codaCollettori[2], (void*)&MSG, sizeof(Messaggio)-sizeof(long), 0);
		
		if(error < 0)
		{
			perror("[THREAD LETTORE 3] Error to SEND codaCollettori[2]\n");
			exit(1);
		}
		 else
		 	printf("[THREAD LETTORE 3] <%lu> SEND to codaCollettori[2] riuscita\n", i);
	}
	
	pthread_exit(NULL);
}

void lettura(MonitorAggregatore* Monitor, int *val)
{
	pthread_mutex_lock(&(Monitor->Mutex));
	
	while(Monitor->numScrittori > 0)
	{
		Monitor->waitLettori++;
		pthread_cond_wait(&(Monitor->CondVarLettori), &(Monitor->Mutex));
		Monitor->waitLettori--;
	}
	
	Monitor->numLettori++;
	
	pthread_mutex_unlock(&(Monitor->Mutex));
	
	*val = Monitor->valore;
	printf("Aggregatore: Lettura valore=%d\n", *val);
	
	pthread_mutex_lock(&(Monitor->Mutex));
	
	Monitor->numLettori--;
	
	if(Monitor->numLettori == 0)
		pthread_cond_signal(&(Monitor->CondVarScrittori));
		
	pthread_mutex_unlock(&(Monitor->Mutex));
}

void scrittura(MonitorAggregatore* Monitor, int val)
{
	pthread_mutex_lock(&(Monitor->Mutex));
	
	while(Monitor->numLettori > 0 || Monitor->numScrittori > 0)
	{
		Monitor->waitScrittori++;
		pthread_cond_wait(&(Monitor->CondVarScrittori), &(Monitor->Mutex));
		Monitor->waitScrittori--;
	}
	
	Monitor->numScrittori++;
	
	pthread_mutex_unlock(&(Monitor->Mutex));
	
	printf("Aggregatore: Scrittura valore=%d\n", val);
	Monitor->valore = val;
	
	pthread_mutex_lock(&(Monitor->Mutex));
	
	Monitor->numScrittori--;
	
	if(Monitor->waitScrittori > 0)
		pthread_cond_signal(&(Monitor->CondVarScrittori));
	 else
	 	pthread_cond_broadcast(&(Monitor->CondVarLettori));
		
	pthread_mutex_unlock(&(Monitor->Mutex));
}
