#ifndef _HEADER_H
#define _HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <pthread.h>

struct MonitorNave {
	int Molo;
	int ID_Nave;

	int numLettori;
	int Occupato;

	int codaLettori;
	int codaScrittori;

	pthread_mutex_t Mutex;
	pthread_cond_t Aggiorna;
	pthread_cond_t Leggi;
};

void Inizializza(struct MonitorNave*);
void Rimuovi(struct MonitorNave*);
void ScriviMolo(struct MonitorNave*, int);
int LeggiMolo(struct MonitorNave*);

#endif
