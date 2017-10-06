#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <stdint.h>

#include "defs.h"

#define RUNNING 0
#define READY 1
#define BLOCKED 2
#define BLOCKED_READ 3
#define DELETE 4
#define BLOCKED_FOREGROUND 5

#define MAX_FDS 64
#define MAX_DATA_PAGES 64
#define MAX_PROCESS_NAME 64


typedef struct c_process process;
typedef char status;

void initialize_process_mutex();

process * create_process(uint64_t rip, uint64_t params, const char * name);
void destroy_process(process * p);

void set_rsp_process(process * p, uint64_t rsp);
uint64_t get_rsp_process(process * p);

void block_process(process * p);
void unblock_process(process * p);
int is_blocked_process(process * p);
void unblock_read_process(process * p);
void block_read_process(process * p);
void block_foreground_process(process * p); /* Se desbloquea al ahcer set_foreground */

uint64_t pid_process(process * p);
uint64_t ppid_process(process * p);
uint64_t number_processes();

process * get_process_by_pid (uint64_t pid);

/* Quizas no tengan que estar aca */
void set_foreground_process (process *p);
void set_foreground_force_process (process * p);
process * get_foreground_process();

/* kill settea que hay que borrar el proceso. No lo borra. is_delete devuelve 1 si hay que borrarlo. */
int kill_process(process * p);
int is_delete_process(process * p);

/* Archivos del proceso */
int set_file_open(process * p, int fd);
int set_file_closed(process * p, int fd);
int file_is_open(process * p, int fd);

/* Paginas de datos que devuelve el memory allocator */
void add_data_page(process * p, void * page);
void remove_data_page(process * p, void * page);

void * stack_page_process(process * p);
void data_pages_process(process * p, void * page_array[]);

int get_name_process(char * buffer, process * p);

/* Devuelve los pids actuales. Termina el arreglo con -1 */
int get_current_pids(int pid_array[]);

#endif
