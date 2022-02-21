#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

#include "header.h"

#define NUM_THREADS_OPERANDI    2
#define NUM_THREADS_CALCOLO     3

int coda_risultati;

int main(){

        pthread_attr_t attr;
        pthread_t threads_operandi[NUM_THREADS_OPERANDI];
        pthread_t threads_calcolo[NUM_THREADS_CALCOLO];
    
        srand(time(NULL)*getpid());

        coda_risultati = msgget(IPC_PRIVATE, IPC_CREAT|0664);
        printf("Coda coda_risultati create con desc: %d\n", coda_risultati);
    
        initServiceQueues();
            
        // Creazione di una istanza di struttura monitor per gli operandi
        // MonitorOperandi sarà usato dai thread generazione operandi
        MonitorOperandi * pc_op = malloc(sizeof(MonitorOperandi));
    
        //Inizializzazione del Mutex e delle condition variables
        pthread_mutex_init(&pc_op->mutex, NULL);
        
        pthread_cond_init(&pc_op->condVarProduttori, NULL);
        pthread_cond_init(&pc_op->condVarConsumatori, NULL);
            
        //Inizializzazione delle variabili di stato
        pc_op->codaOperandi = 0;
        pc_op->testaOperandi = 0;
        pc_op->numOperandi = 0;

        //Impostazione dei thread come joinable
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

        //Generazione del processo prelievo risultati
        pid_t pid;
        
        pid = fork();
        
        if(pid == 0)
        {
        	printf("Processo prelievo risultati creato... con PID %d\n", getpid());
        	preleva_risultati(coda_risultati);
        }
    
        //Generazione dei thread Operandi
        for(size_t i=0; i<NUM_THREADS_OPERANDI; i++)
        	pthread_create(&threads_operandi[i], &attr, genera_operandi, (void*) pc_op);
    
        //Generazione dei thread di Calcolo
        for(size_t i=0; i<NUM_THREADS_CALCOLO; i++)
        	pthread_create(&threads_calcolo[i], &attr, calcola, (void*) pc_op);
    
        //Join threads a wait processo
        for(size_t i=0; i<NUM_THREADS_OPERANDI; i++)
        	pthread_join(threads_operandi[i],NULL);
        	
        for(size_t i=0; i<NUM_THREADS_CALCOLO; i++)
        	pthread_join(threads_calcolo[i], NULL);
        	
        wait(NULL);
    
        //Deallocazione risorse
        pthread_attr_destroy(&attr);
        pthread_mutex_destroy(&pc_op->mutex);
        pthread_cond_destroy(&pc_op->condVarConsumatori);
        pthread_cond_destroy(&pc_op->condVarProduttori);
        
        free(pc_op);
        
        msgctl(coda_risultati, IPC_RMID, NULL);
        
        removeServiceQueues();
    
        pthread_exit(NULL);
}

