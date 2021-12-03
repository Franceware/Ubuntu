#include "StazioneMeteo.h"

#define NUM_SCRITTURE 20
#define NUM_LETTURE 10

void InizioLettura(MonitorMeteo* monMet)
{
	enter_monitor(&(monMet->mon));

	while(monMet->numScrittori > 0)
		wait_condition(&(monMet->mon), CONDVARLETTORI);
	
	monMet->numLettori = monMet->numScrittori + 1;

	signal_condition(&(monMet->mon), CONDVARLETTORI);
}

void FineLettura(MonitorMeteo* monMet)
{
	enter_monitor(&(monMet->mon));

	monMet->numLettori = monMet->numLettori - 1;

	if(monMet->numLettori == 0)
		signal_condition(&(monMet->mon), CONDVARSCRITTORI);
     else
		leave_monitor(&(monMet->mon));
}

void InizioScrittura(MonitorMeteo* monMet)
{
	enter_monitor(&monMet->mon);

	while(monMet->numLettori > 0 || monMet->numScrittori > 0)
		wait_condition(&(monMet->mon), CONDVARSCRITTORI);

	monMet->numScrittori = 1;

	leave_monitor(&(monMet->mon));
}

void FineScrittura(MonitorMeteo* monMet)
{
	enter_monitor(&monMet->mon);

	monMet->numScrittori = 0;

	if(queue_condition(&(monMet->mon), CONDVARSCRITTORI) > 0)
		signal_condition(&(monMet->mon), CONDVARSCRITTORI);
	 else if(queue_condition(&(monMet->mon), CONDVARLETTORI))
		 signal_condition(&(monMet->mon), CONDVARLETTORI);
	       else
	    		leave_monitor(&(monMet->mon));
}

void Servizio(MonitorMeteo * monMet)
{
	srand(time(NULL));

	for(size_t i=0; i<NUM_SCRITTURE; i++)
	{
		InizioScrittura(monMet);

		monMet->meteo.Temperatura = (rand()%101) - 50;
		monMet->meteo.Umidita = rand()%101;
		monMet->meteo.Pioggia = rand()%2;

		printf("[SERVIZIO] <%d> Scrittura: Temperatura = %d, Umidità = %d, Pioggia = %d\n", getpid(), monMet->meteo.Temperatura, monMet->meteo.Umidita, monMet->meteo.Pioggia);

		FineScrittura(monMet);

		sleep(2);
	}
}

void Utente(MonitorMeteo * monMet)
{
	for(size_t i=0; i<NUM_LETTURE; i++)
	{
		InizioLettura(monMet);

		printf("[UTENTE] <%d> Lettura: Temperatura = %d, Umidità = %d, Pioggia = %d\n", getpid(), monMet->meteo.Temperatura, monMet->meteo.Umidita, monMet->meteo.Pioggia);

		FineLettura(monMet);

		sleep(1);
	}
}
