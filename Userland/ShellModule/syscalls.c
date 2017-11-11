#include "syscalls.h"
#include "ipc_info.h"
#include "process_info.h"

/* Ambas llamadas son llamadas desde "stdio.c" y llaman funciones de "stdio.asm" */

/* SystemCall Read */
int read(unsigned int fd, char *buff, unsigned int bytes) {
     int i;
     readC(buff,bytes);
     return i;
}

/* SystemCall Write */
int write(unsigned int fd, char buff, unsigned int bytes) {
    int i;
    writeC(buff,bytes);
    return i;
}

/* SystemCall Time */
int time(int selector) {
    return timeC(selector);
}

/* Syscall Clear Screen */
void clear_screen(){
    clearC();
}

int exec(void * ptr, uint64_t params, const char * name) {
  return execC((uint64_t) ptr, params, (uint64_t) name);
}

/* SystemCall Wait */
int wait(unsigned long int milliseconds) {
    return waitC(milliseconds);
}

/* SystemCall para reservar espacio en memoria*/
void * reserve (unsigned int bytes) {
    return (void *) reserveC(bytes);
}

/* SystemCall para reservar espacio en memoria*/
int free_reserve (void * address) {
    return freeC((uint64_t) address);
}

/* SystemCall para obtener la direccion de memoria de datos */
void * data_address() {
    return (void *) dataC();
}

void end() {
  endC();
}

void yield() {
  yieldC();
}

int mutex_open(char * name) {
  return mutex_opC((uint64_t) name);
}

int mutex_close(int key) {
  return mutex_clC((uint64_t) key);
}

int mutex_lock(int key) {
  return mutex_loC((uint64_t) key);
}

int mutex_unlock(int key) {
  return mutex_ulC((uint64_t) key);
}

int set_foreground(uint64_t pid) {
  return set_fgC(pid);
}

int fifo_open(char * name) {
  return fifo_opC((uint64_t) name);
}

int fifo_close(int key) {
  return fifo_clC((uint64_t) key);
}

int kill(uint64_t pid) {
  return killC(pid);
}

int pid() {
  return pidC();
}

int ppid() {
  return ppidC();
}

int get_process_info(uint64_t pid, struct process_info_c * pi) {
  return get_process_infoC(pid, (uint64_t) pi);
}

int cond_open(char * name) {
  return cond_opC((uint64_t) name);
}

int cond_wait(int cond_key, int lock_key) {
  return cond_waitC(cond_key, lock_key);
}

int cond_signal(int key) {
  return cond_signalC(key);
}

int cond_broadcast(int key) {
  return cond_bcC(key);
}

int cond_close(int key) {
  return cond_clC(key);
}

int get_current_pids(int * pid_array) {
  return get_current_pidsC((uint64_t) pid_array);
}

int get_conds_info(cond_info info_array[]) {
  return get_conds_infoC((uint64_t) info_array);
}

int get_mutexes_info(mutex_info info_array[]) {
  return get_mutexes_infoC((uint64_t) info_array);
}

int get_fifos_info(fifo_info info_array[]) {
  return get_fifos_infoC((uint64_t) info_array);
}
