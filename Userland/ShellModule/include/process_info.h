#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <stdint.h>

#define MAX_PROCESS_NAME 64
#define MAX_MEM_PAGES 64
#define MAX_PROCESS_STRING 256

#define MAX_PROCESSES 128

#define RUNNING 0
#define READY 1
#define BLOCKED 2


typedef struct process_info_c {
	char is_foreground;
	char st;
	uint64_t pid;
	uint64_t ppid;
	char name [MAX_PROCESS_NAME];
	void * stack_address;
	void * mem_pages[MAX_MEM_PAGES];
} process_info;

/* Hace un toString del proceso. Se pasa un puntero para que sea más eficiente. 
** Devuelve la longitud. */
int process_string(const process_info * p, char buffer[MAX_PROCESS_STRING]);

#endif