#ifndef _MUTEX_H_
#define _MUTEX_H_

#include "defs.h"

#define MAX_MUTEXES 128

#define MUTEX_NAME_LEN 64

#define OPEN 1
#define CLOSED 0

#define NOT_OPEN_ERROR -1
#define MAX_MUTEX_OPEN_ERROR -2

int mutex_open(char * name);
int mutex_lock(int key);
int mutex_unlock(int key);
int mutex_close(int key);

#endif
