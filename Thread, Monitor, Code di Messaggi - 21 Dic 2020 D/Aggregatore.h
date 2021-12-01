#ifndef _AGGREGATORE_H
#define _AGGREGATORE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <errno.h>

#include <pthread.h>

#include "Sensore.h"

#define NUM_COLLETTORI 3
#define NUM_LETTORI 3

typedef struct 
{
	int valore;

	int numScrittori;
	int numLettori;
	int waitLettori;
	int waitScrittori;
	
	int codaSensori;
	int codaCollettori[NUM_COLLETTORI];

	pthread_mutex_t Mutex;
	pthread_cond_t CondVarLettori;
	pthread_cond_t CondVarScrittori;
}MonitorAggregatore;

void aggregazione(int, int*);
void * threadScrittore(void *);
void * threadLettore1(void *);
void * threadLettore2(void *);
void * threadLettore3(void *);
void lettura(MonitorAggregatore*, int*);
void scrittura(MonitorAggregatore*, int);

#endif
