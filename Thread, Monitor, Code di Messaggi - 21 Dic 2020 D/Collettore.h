#ifndef _COLLETTORE_H
#define _COLLETTORE_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <errno.h>

void collettore(int idQueueCollettore);

#endif
