#include "driverVideo.h"
#include "driverKeyboard.h"
#include "stdlib.h"
#include "systemCalls.h"
#include "timer.h"
#include "condVariable.h"
#include "mutex.h"
#include "process.h"
#include "fifo.h"
#include "ipc_info.h"
#include "process_info.h"

#define INIT 0
#define SHELL 1

static unsigned int fifo_to_fds(int key) {
	return key + FILE_DESCRIPTORS;
}

static int fds_to_fifo(unsigned int fds) {
	return fds - FILE_DESCRIPTORS;
}

/*Syscall read y write llamadas desde libasm, se conectan directamente con las funciones del Kernel*/
/* SystemCall de Read para leer de entrada estandar*/
/* el int que se retorna permite chequear si consumimos todo el buffer de teclado*/
uint64_t sys_read(uint64_t fds, char * buffer, uint64_t bytes) {
	unsigned int i = 0;
	char c;
	int block=-1;
		c = poll_keyboard_buffer(buffer,bytes); // con la implementacion actual se lee de a un caracter
		if (c != '\0') {
			block=0;
		}
		else{
			block= -1;
		}

		// con la implementacion actual se lee de a un caracter
		/*
		if(fds == STDIN){
			c = poll_keyboard_buffer(buffer,bytes); 
			if (c != '\0') {
				block=0;
			} 
			else{
				block= -1;
			}
		}
		else if (fds >= FILE_DESCRIPTORS){
			i=fifo_read(fds_to_fifo(fds), buffer, bytes);
		}
		*/
    return block;
}

/* SystemCall de Write para escribir a salida estándar */
uint64_t sys_write(uint64_t fds, const char  str, uint64_t length) {
	int n;

	if (fds == STDERR) { // podriamos darle un color especial para diferenciar los errores
		printString(str);
	}
	else if (fds == STDOUT) {
		printChar(str);
	}
	/*
	else if (fds >= FILE_DESCRIPTORS){
		n=fifo_write(fds_to_fifo(fds), str, length);
	}
	*/
	return 0;
}

uint64_t sys_clear(){
	clear();
	return 0;
}

/* SystemCall de Time, retorna hora, minutos y segundos actuales */
uint64_t sys_time(uint64_t selection) {
    switch(selection) {
	case 0:
	    return seconds();
	case 1:
	    return minutes();
	case 2:
	    return hour();
    }
    return -1;
}

/* Nos permite ejecutar procesos*/
uint64_t sys_exec(uint64_t ptr, uint64_t params, const char * name) {
	return exec_process(create_process(ptr, params, name));
}

/* SystemCall de wait, Pone en espera a la pantalla por la cantidad de milisegundos pasados por parámetro*/
uint64_t sys_wait(uint64_t milliseconds) {
	return sleep(milliseconds);
}

/* SystemCall Malloc */
uint64_t sys_malloc(uint64_t bytes) {
	void * page = (void *) get_page(bytes);
	add_data_page(get_current_process(), page);
	return (uint64_t) page;
}

/* System call free*/
uint64_t sys_free(uint64_t address) {
	remove_data_page(get_current_process(), (void *) address);
	return store_page(address);
}

/* System call que retorna la dirección del módulo de datos */
uint64_t sys_data_address() {
	return DATA_DIR;
}

uint64_t sys_end() {
	end_process();
	return 1;
}

uint64_t sys_yield() {
	yield_process();
	return 1;
}

uint64_t sys_mutex_op(uint64_t nameptr) {
	return mutex_open((char *)nameptr);
}

uint64_t sys_mutex_cl(uint64_t key) {
	return mutex_close(key);
}

uint64_t sys_mutex_lock(uint64_t key) {
	return mutex_lock(key);
}

uint64_t sys_mutex_unlock(uint64_t key) {
	return mutex_unlock(key);
}

uint64_t sys_set_foreground(uint64_t pid) {
	process * p = get_process_by_pid(pid);

	if (p == NULL)
		return 0;

	set_foreground_process(p);

	return 1;
}

uint64_t sys_fifo_op(uint64_t nameptr) {
	return fifo_to_fds(fifo_open((char *)nameptr));
}

uint64_t sys_fifo_cl(uint64_t fds) {
	return fifo_close(fds_to_fifo(fds));
}


uint64_t sys_kill(uint64_t pid) {
	int valid = 0;

	if (pid != INIT && pid != SHELL) {
		process * p = get_process_by_pid(pid);
		valid = kill_process(p);
	}

	return valid;
}


uint64_t sys_pid() {
	return pid_process(get_current_process());
}
uint64_t sys_ppid() {
	return ppid_process(get_current_process());
}

uint64_t sys_process_info(uint64_t pid, struct process_info_c * pi) {
	return get_process_info_by_pid(pi, pid);
}

uint64_t sys_cond_open(char * name) {
	return cond_open(name);
}

uint64_t sys_cond_wait(int cond_key, int lock_key) {
	return cond_wait(cond_key, lock_key);
}

uint64_t sys_cond_signal(int key) {
	return cond_signal(key);
}

uint64_t sys_cond_broadcast(int key) {
	return cond_broadcast(key);
}

uint64_t sys_cond_close(int key) {
	return cond_close(key);
}

uint64_t sys_get_pids(int * pid_array) {
	return get_current_pids(pid_array);
}

uint64_t sys_get_conds_info(cond_info info_array[]) {
	return get_conds_info(info_array);
}

uint64_t sys_get_mutexes_info(mutex_info info_array[]) {
	return get_mutexes_info(info_array);
}

uint64_t sys_get_fifos_info(fifo_info info_array[]) {
	return get_fifos_info(info_array);
}




