#include "Molo.h"

void Inizializza(struct MonitorNave* Monitor)
{
	Monitor->Molo = 0;
	Monitor->ID_Nave = 0;

	Monitor->Occupato = 0;
	Monitor->numLettori = 0;

	Monitor->codaLettori = 0;
	Monitor->codaScrittori = 0;

	pthread_mutex_init(&(Monitor->Mutex), NULL);
	pthread_cond_init(&(Monitor->Aggiorna), NULL);
	pthread_cond_init(&(Monitor->Leggi), NULL);
}

void Rimuovi(struct MonitorNave* Monitor)
{
	pthread_mutex_destroy(&(Monitor->Mutex));
	pthread_cond_destroy(&(Monitor->Aggiorna));
	pthread_cond_destroy(&(Monitor->Leggi));
}

void ScriviMolo(struct MonitorNave* Monitor, int Molo)
{
	pthread_mutex_lock(&(Monitor->Mutex));

	while(Monitor->Occupato == 1)
	{
		Monitor->codaScrittori = Monitor->codaScrittori + 1;
		pthread_cond_wait(&(Monitor->Aggiorna), &(Monitor->Mutex));
		Monitor->codaScrittori = Monitor->codaScrittori - 1;
	}

	Monitor->Occupato = 1;

	pthread_mutex_unlock(&(Monitor->Mutex));

	//Scrittura
	Monitor->Molo = Molo;

	pthread_mutex_lock(&(Monitor->Mutex));

	if(Monitor->codaScrittori > 0)
		pthread_cond_signal(&(Monitor->Aggiorna));
	 else if(Monitor->codaLettori > 0)
		 pthread_cond_signal(&(Monitor->Leggi));

	 Monitor->Occupato = 0;

	 pthread_mutex_unlock(&(Monitor->Mutex));
}

int LeggiMolo(struct MonitorNave* Monitor)
{
	int Result;

	pthread_mutex_lock(&(Monitor->Mutex));

	while(Monitor->Occupato == 1 && Monitor->numLettori == 0)
	{
		Monitor->codaLettori = Monitor->codaLettori + 1;
		pthread_cond_wait(&(Monitor->Leggi), &(Monitor->Mutex));
	}

	Monitor->Occupato = 1;

	Monitor->numLettori = Monitor->numLettori + 1;

	pthread_cond_signal(&(Monitor->Leggi));

	pthread_mutex_unlock(&(Monitor->Mutex));

	//Lettura
	Result = Monitor->Molo;

	pthread_mutex_lock(&(Monitor->Mutex));

	Monitor->numLettori = Monitor->numLettori - 1;

	if(Monitor->numLettori == 0)
	{
		Monitor->Occupato = 0;
		pthread_cond_signal(&(Monitor->Aggiorna));
	}

	pthread_mutex_unlock(&(Monitor->Mutex));

	return Result;
}
