#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include "process.h"
#include "scheduler.h"
#include "stdlib.h"
#include <stdint.h>

typedef struct process_info_c {
	char is_foreground;
	char st;
	uint64_t pid;
	uint64_t ppid;
	char name [MAX_PROCESS_NAME];
	void * stack_address;
	void * mem_pages[MAX_DATA_PAGES];
} process_info;

/* Copia la información de un proceso a la estructura pública process_info. 
** Dicha estructura funciona como interfaz entre Kernel y Userland para leer
** la información de un proceso */
void get_process_info (process_info * pi, process * p);

int get_process_info_by_pid(process_info * pi, uint64_t pid);
#endif