#include "Treno.h"

#define NUM_TRENI 4
#define NUM_VIAGGIATORI 10

void * OpCapoTreno (void * Tr)
{
	int Station;
	struct MonitorTreno* Train = (struct MonitorTreno*) Tr;
	
	Station = Train->Stazione;
	
	for(size_t i=0; i<NUM_VIAGGIATORI; i++)
	{
		Station = Station + 1;
		ScritturaStazione(Train, Station);
		
		printf("[CAPOTRENO: Posizione del treno %d monificata --> NUOVA POSIZIONE: %d]\n", Train->ID_Treno, Station);
		sleep(3);
	}
	
	pthread_exit(NULL);
}

void * OpViaggiatori (void * Tr)
{
	int Station;
	struct MonitorTreno* Train = (struct MonitorTreno*) Tr;
	
	for(size_t i=0; i<3; i++)
	{
		sleep(rand()%6 +1);
		
		Station = LeggiStazione(Train);
		
		printf("[VIAGGIATORE: Il treno n° %d e' nella %d stazione\n", Train->ID_Treno, Station);
	}
	
	pthread_exit(NULL);
}


int main()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	
	srand(time(NULL));

	struct MonitorTreno* Treni[NUM_TRENI];

	pthread_t CapoTreno[NUM_TRENI];
	pthread_t Viaggiatori[NUM_VIAGGIATORI];

	for(size_t i=0; i<NUM_TRENI; i++)
		Treni[i] = (struct Monitor*) malloc(sizeof(struct MonitorTreno));

	for(size_t i=0; i<NUM_TRENI; i++)
		Inizializza(Treni[i]);
		
	//Assegnazione ID
	Treni[0]->ID_Treno = 1;
	Treni[1]->ID_Treno = 2;
	Treni[2]->ID_Treno = 3;
	Treni[3]->ID_Treno = 4;
	
	//Creazione dei Thread Capitreno
	for(size_t i=0; i<NUM_TRENI; i++)
		pthread_create(&CapoTreno[i], NULL, OpCapoTreno,(void*) Treni[i]);
		
	for(size_t i=0; i<NUM_VIAGGIATORI; i++)
		pthread_create(&Viaggiatori[i], NULL, OpViaggiatori, (void*) Treni[rand()%NUM_TRENI]);
		
	//Join Threads
	for(size_t i=0; i<NUM_VIAGGIATORI; i++)
	{
		pthread_join(Viaggiatori[i], NULL);
		printf("Viaggiatore %lu terminato\n", i+1);
	}
	
	for(size_t i=0; i<NUM_TRENI; i++)
		Rimuovi(Treni[i]);
		
	for(size_t i=0; i<NUM_TRENI; i++)	free(Treni[i]);
	
	return 0;	
}
