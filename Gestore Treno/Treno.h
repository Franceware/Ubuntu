#ifndef _HEADER_H
#define _HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

struct MonitorTreno
{
	int Stazione;
	int ID_Treno;

	int numLettori;
	int Occupato;

	int codaLettori;
	int codaScrittori;

	pthread_mutex_t Mutex;
	pthread_cond_t Aggiorna;
	pthread_cond_t Leggi;
};

void Inizializza(struct MonitorTreno*);
void Rimuovi(struct MonitorTreno*);
void ScritturaStazione(struct MonitorTreno*, int);
int LeggiStazione(struct MonitorTreno*);

#endif
