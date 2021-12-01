#ifndef _GESTIONECHAT_H
#define _GESTIONECHAT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include <string.h>

#define FTOK_PATH "."
#define MSG_TYPE 1

#define DIMENSION_MESSAGE 20

typedef struct
{
	long type;
	char msg[DIMENSION_MESSAGE];
} Message;

void Sender(int, int);
void Reicever(int);

#endif
