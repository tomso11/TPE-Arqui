#ifndef _IPC_INFO_
#define _IPC_INFO_

#include "mutex.h"
#include "condVariable.h"
#include "fifo.h"

typedef struct mutex_info {
	char name[MUTEX_NAME_LEN];
	int key;
	int locked;
} mutex_info;

typedef struct cond_info {
	char name[COND_NAME_LEN];
	int key;
} cond_info;

typedef struct fifo_info {
	char name[FIFO_NAME_LEN];
	int fds;
} fifo_info;

int get_conds_info(cond_info info_array[]);
int get_mutexes_info(mutex_info info_array[]);
int get_fifos_info(fifo_info info_array[]);

#endif