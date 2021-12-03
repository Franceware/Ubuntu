#include "MonitorHoare.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <time.h>

#define CONDVARLETTORI 0
#define CONDVARSCRITTORI 1
#define VARCONDTOTALI 2

typedef struct {
	int Temperatura;
	unsigned int Umidita;
	unsigned short Pioggia;
} Meteo;

typedef struct{

	Meteo meteo;
	int numLettori;
	int numScrittori;

	Monitor mon;

}MonitorMeteo;

void Servizio(MonitorMeteo*);
void Utente(MonitorMeteo*);
