#ifndef _SYSTEMCALLS_H_
#define _SYSTEMCALLS_H_

#include <stdint.h>
#include "driverVideo.h"

#define STDOUT 0
#define STDIN 1
#define STDERR 2

/*Direccion de inicio del módulo de datos */
#define DATA_DIR 0x500000


uint64_t sys_read(uint64_t fds, char * buffer, uint64_t bytes);
uint64_t sys_write(uint64_t fds, const char  str, uint64_t length);
uint64_t sys_malloc(uint64_t bytes);
uint64_t sys_exec(uint64_t ptr, uint64_t params, const char * name);
uint64_t sys_data_dir();
uint64_t sys_time(uint64_t selection);
uint64_t sys_clear();
uint64_t sys_wait(uint64_t milliseconds);
uint64_t sys_end(); 
uint64_t sys_yield(); 
uint64_t sys_mutex_op(uint64_t nameptr);
uint64_t sys_mutex_cl(uint64_t key);
uint64_t sys_mutex_lock(uint64_t key);
uint64_t sys_mutex_unlock(uint64_t key);
uint64_t sys_fifo_op(uint64_t nameptr);
uint64_t sys_fifo_cl(uint64_t fds);
uint64_t sys_set_foreground(uint64_t pid);
uint64_t sys_kill(uint64_t pid);
uint64_t sys_pid();
uint64_t sys_ppid();
uint64_t sys_process_info(uint64_t pid, struct process_info_c * pi);
uint64_t sys_cond_open(char * name);
uint64_t sys_cond_wait(int cond_key, int lock_key);
uint64_t sys_cond_signal(int key);
uint64_t sys_cond_broadcast(int key);
uint64_t sys_cond_close(int key);
uint64_t sys_get_pids(int * pid_array);
uint64_t sys_get_conds_info(cond_info info_array[]);
uint64_t sys_get_mutexes_info(mutex_info info_array[]);
uint64_t sys_get_fifos_info(fifo_info info_array[]);
uint64_t sys_free(uint64_t address);

#endif
