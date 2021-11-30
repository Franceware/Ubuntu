#include "Molo.h"

#define NUM_MOLI 5
#define NUM_VIAGGIATORI 10

void * gestoreMolo (void * Ml)
{
	int M;
	struct MonitorNave* Mol = (struct MonitorNave*) Ml;
	
	M = Mol->Molo;
	
	for(size_t i=0; i<NUM_VIAGGIATORI; i++)
	{
		M = M + 1;
		ScriviMolo(Mol, M);
		
		printf("[C: Posizione della nave %d modificata. La nuova posizione e': %d]\n", Mol->ID_Nave, M);
		sleep(3);
	}
	
	pthread_exit(NULL);
}

void * OpViaggiatori (void * Ml)
{
	int M;
	struct MonitorNave* Mol = (struct MonitorNave*) Ml;
	
	for(size_t i=0; i<3; i++)
	{
		sleep(rand()%6 +1);
		
		M = LeggiMolo(Mol);
		
		printf("[V] La nave n.ro %d e' nel molo %d\n", Mol->ID_Nave, M);
	}
	
	pthread_exit(NULL);
}


int main()
{
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	
	srand(time(NULL));

	struct MonitorNave* Moli[NUM_MOLI];

	pthread_t GestoreMolo[NUM_MOLI];
	pthread_t Viaggiatori[NUM_VIAGGIATORI];

	for(size_t i=0; i<NUM_MOLI; i++)
		Moli[i] = (struct MonitorNave*) malloc(sizeof(struct MonitorNave));

	for(size_t i=0; i<NUM_MOLI; i++)
		Inizializza(Moli[i]);
		
	//Assegnazione ID
	Moli[0]->ID_Nave = 1;
	Moli[1]->ID_Nave = 2;
	Moli[2]->ID_Nave = 3;
	Moli[3]->ID_Nave = 4;
	Moli[4]->ID_Nave = 5;
		
	//Creazione dei Thread Capitreno
	for(size_t i=0; i<NUM_MOLI; i++)
		pthread_create(&GestoreMolo[i], NULL, gestoreMolo,(void*) Moli[i]);
		
	for(size_t i=0; i<NUM_VIAGGIATORI; i++)
		pthread_create(&Viaggiatori[i], NULL, OpViaggiatori, (void*) Moli[rand()%NUM_MOLI]);
	
	for(size_t i=0; i<NUM_MOLI; i++)
	{
		pthread_join(GestoreMolo[i], NULL);
		printf("Gestore Molo %lu terminato\n", i+1);
	}	
	//Join Threads
	for(size_t i=0; i<NUM_VIAGGIATORI; i++)
	{
		pthread_join(Viaggiatori[i], NULL);
		printf("Viaggiatore %lu terminato\n", i+1);
	}
	
	for(size_t i=0; i<NUM_MOLI; i++)
		Rimuovi(Moli[i]);
		
	for(size_t i=0; i<NUM_MOLI; i++)	free(Moli[i]);
	
	return 0;	
}
