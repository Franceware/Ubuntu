#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

#define DIM_BUFFER 5
#define DIM_MESSAGE 20

#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2
#define RISULTATO_TYPE 3

extern int coda_OTS;
extern int coda_RTS; 
extern int coda_risultati;

typedef struct 
{
    //Pool di buffer condivisi
    int operandi[DIM_BUFFER];
    
    //Variabili per la gestione circolare del buffer operandi
    int testaOperandi;
    int codaOperandi;
    
    //Contatore disponibilità per il prelievo
    int numOperandi;
    
    //Dichiarazione variable Condition e Mutex per il problema Produttore/Consumatore
    pthread_mutex_t mutex;
    pthread_cond_t condVarConsumatori; 
    pthread_cond_t condVarProduttori;
} MonitorOperandi;

typedef struct 
{
    long type;
    char message[DIM_MESSAGE];
} Risultato;

void inserisci_operando(MonitorOperandi * mo, int operando);
int *preleva_operando(MonitorOperandi * mo);

void inserisci_risultato(Risultato *m , int queue);
int preleva_risultato(Risultato *m, int queue, int tipomess);

void * genera_operandi(void *);
void * calcola(void *);
void preleva_risultati(int);

void initServiceQueues();
void removeServiceQueues();
    
#endif

