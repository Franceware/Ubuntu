#ifndef _SENSORE_H
#define _SENSORE_H

#define MSG_TYPE 1
#define NUM_GENERAZIONI 10

typedef struct
{
	long type;
	int lettura;
} Messaggio;

void generaLettura(int);

#endif
