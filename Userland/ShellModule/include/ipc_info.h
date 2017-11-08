#ifndef _IPC_INFO_
#define _IPC_INFO_

#define MAX_COND_VAR 128
#define MAX_FIFOS 128
#define MAX_MUTEXES 128

#define MUTEX_NAME_LEN 64
#define COND_NAME_LEN 64
#define FIFO_NAME_LEN 64


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

void print_ipc_info();
void print_conds_info(cond_info info_array[], int count);
void print_mutexes_info(mutex_info info_array[], int count);
void print_fifos_info(fifo_info info_array[], int count);

#endif