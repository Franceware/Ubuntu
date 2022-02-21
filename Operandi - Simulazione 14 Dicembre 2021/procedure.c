#include "header.h"
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <string.h>

int coda_RTS;
int coda_OTS;
int coda_risultati;

void initServiceQueues()
{
    //Inizializzazione code per effettuare l'handshaking (sicronizzazione con send sincrona e reiceve bloccante)
  	//Coda RTS --> coda per la Request to Send
  	//Coda OTS --> coda per la Ok to Send	
  	
    coda_RTS = msgget(IPC_PRIVATE,IPC_CREAT|0664);
    printf("Coda di servizio coda_RTS create con desc: %d\n", coda_RTS);
    
    coda_OTS = msgget(IPC_PRIVATE, IPC_CREAT|0664);
    printf("Coda di servizio coda_OTS create con desc: %d\n", coda_OTS);
}

void removeServiceQueues()
{
    //Rimozione code di servizio
    msgctl(coda_RTS, IPC_RMID, NULL);
    msgctl(coda_OTS, IPC_RMID, NULL); 
}

// Send Sincrona
void inserisci_risultato (Risultato *m , int queue)
{
    Risultato msgA, msgB;
    
    //Costruzione del messaggio msgA da inviare alla coda RTS
    msgA.type = REQUEST_TO_SEND;
    strcpy(msgA.message, "Richiesta d'invio");
    
    //Invio del messaggio alla coda RTS (Request to send)
    msgsnd(coda_RTS, &msgA, sizeof(Risultato) - sizeof(long), 0);
    printf("Messaggio inviato alla coda %d Type: %lu Message: %s\n", coda_RTS, msgA.type, msgA.message);
    
    //Ricezione del messaggio sulla coda OTS (Ok to Send)
    msgrcv(coda_OTS, &msgB, sizeof(Risultato) - sizeof(long), OK_TO_SEND, 0);
    printf("Messaggio ricevuto sulla coda %d Type: %lu Message: %s\n", coda_OTS, msgB.type, msgB.message);
    
    //Invio del messaggio effettivo alla coda queue
    msgsnd(queue, m, sizeof(Risultato) - sizeof(long), 0);    
    printf("Messaggio inviato alla coda %d Type: %lu Message: %s\n", coda_OTS, m->type, m->message);
}

// Receive Bloccante
int preleva_risultato(Risultato *m, int queue, int tipomess)
{
    Risultato msgA, msgB;
    
    //Ricezione del messaggio sulla coda RTS (Request to Send)
    msgrcv(coda_RTS, &msgA, sizeof(Risultato) - sizeof(long), REQUEST_TO_SEND, 0);
    printf("Messaggio ricevuto sulla coda %d Type: %lu Message: %s\n", coda_RTS, msgA.type, msgA.message);
    
    //Costruzione del messaggio da inviare sulla coda OTS (Ok to Send)
    msgB.type = OK_TO_SEND;
    strcpy(msgB.message, "Pronto per l'invio");
    
    //Invio del messaggio sulla coda OTS (Ok to Send)
    msgsnd(coda_OTS, &msgB, sizeof(Risultato) - sizeof(long), 0);
    printf("Messaggio inviato sulla coda %d Type: %lu Message: %s\n", coda_OTS, msgB.type, msgB.message);
    
    //Ricezione del messaggio effettivo sulla coda queue
    msgrcv(queue, m, sizeof(Risultato) - sizeof(long), tipomess, 0);
    printf("Messaggio ricevuto sulla coda %d Type: %lu Message: %s\n", queue, m->type, m->message);
    
    return atoi(m->message);
}

void inserisci_operando(MonitorOperandi * m, int operando)
{
	//Accesso al mutex
     pthread_mutex_lock(&m->mutex);
     
     //Controllo dello spazio disponibile all'interno del buffer
     while(m->numOperandi == DIM_BUFFER)
     	pthread_cond_wait(&m->condVarProduttori, &m->mutex);
     
     //Inserimento dell'operando generato all'interno del buffer	
     m->operandi[m->codaOperandi] = operando;
     
     printf("[Thread Genera Operandi] Operando prodotto = %d\n", m->operandi[m->codaOperandi]);
     
     m->codaOperandi = (m->codaOperandi+1)%DIM_BUFFER;
     
     m->numOperandi++;
     
     //Risveglio di un processo Consumatore
     pthread_cond_signal(&m->condVarConsumatori);
     
     //Rilascio del mutex
     pthread_mutex_unlock(&m->mutex);
}

int * preleva_operando(MonitorOperandi * m)
{
        static int operando[2];

        //Accesso al mutex 
        pthread_mutex_lock(&m->mutex);
        
        //Verifica della disponibilità di consumazione di minimo 2 elementi all'interno del buffer
        while(m->numOperandi < 2)
        	pthread_cond_wait(&m->condVarConsumatori, &m->mutex);
        
        //Prelievo degli elementi	
        operando[0] = m->operandi[m->testaOperandi];
        m->testaOperandi = (m->testaOperandi+1)%DIM_BUFFER;
        
        operando[1] = m->operandi[m->testaOperandi];
        m->testaOperandi = (m->testaOperandi+1)%DIM_BUFFER;
        
        m->numOperandi = m->numOperandi - 2;
        
        //Risveglio di un processo Produttore
        pthread_cond_signal(&m->condVarProduttori);
        
        //Rilascio del mutex 
        pthread_mutex_unlock(&m->mutex); 
    
        return operando;
}

void * genera_operandi(void *m){

        int i, op;

        MonitorOperandi * mon = (MonitorOperandi*) m;
    
        for (i=0; i<6; i++){
        
                // call inserisci_operando generato da 1 a 10
                op = 1 + rand() % 10;
                //printf("[thread genera_operandi] Operando generato: %d\n", op);
                inserisci_operando(mon, op);
                 
        }
        pthread_exit(0);
}

void preleva_risultati(int coda_risultati){

        int i, risultato, less=0, great=0;
    
        Risultato m;
    
        for (i=0; i<6; i++){
        
            
                //risultato = preleva_risultato(mon);
                risultato = preleva_risultato(&m, coda_risultati, RISULTATO_TYPE);
                printf("[processo preleva_risultati] Risultato prelevato: %d coda: %d\n", risultato, coda_risultati);
            
                if (risultato<=25)
                    less++;
                else
                    great++;
                 
        }
    
        printf("[processo preleva_risultati] STATS: Conteggio minore uguale: risultato: %d\n", less);
        printf("[processo preleva_risultati] STATS: Conteggio maggiore: %d\n", great);
        exit(0);
}


void * calcola(void *mon){

        int i, calc;
        int *op;

        MonitorOperandi * mon_op = (MonitorOperandi*) mon;
    
        Risultato m; //messaggio da inviare
    
        for (i=0; i<2; i++){
        
                // preleva operando
                op = preleva_operando(mon_op);
                int op1 = *op;
                int op2 = *(op+1);
                printf("[thread calcola] Preleva op1: %d op2: %d\n", op1, op2);

                // calcola
                calc = (op1*op1) + (op2*op2);

                printf("[thread calcola] Calcola somma dei quadrati (%d x %d) + (%d x %d) = %d\n", op1, op1, op2, op2, calc);
                // inserisci risultato
            
                printf("[thread calcola] Inserisci risultato: %d tramite send sincrona su coda: %d\n", calc, coda_risultati);
                sleep(1);
            
                //Impostazione del messaggio da inviare
            	m.type = RISULTATO_TYPE;
            	sprintf(m.message, "%d", calc);
                inserisci_risultato(&m , coda_risultati);
        }
    
        pthread_exit(0);
}
