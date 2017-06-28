#include "driverVideo.h"
#include "driverKeyboard.h"
#include "stdlib.h"
#include "systemCalls.h"


/* Vector de system calls */
static uint64_t (*syscalls[]) (uint64_t,uint64_t,uint64_t) = { 0,0,0,/* 0, 1, 2 system calls reservados*/sys_read_wr, sys_write_wr,	sys_time_wr, sys_malloc_wr, sys_data_address_wr, sys_clear_wr};


/* WRAPPERS */
static uint64_t sys_write_wr(uint64_t fds, uint64_t str, uint64_t length) {
	return sys_write(fds, (const char *) str, length);
}

static uint64_t sys_read_wr(uint64_t fds, uint64_t buff, uint64_t bytes) {
	return sys_read(fds, (char *) buff, bytes);
}

static uint64_t sys_malloc_wr(uint64_t selection, uint64_t a, uint64_t b) {
	return sys_malloc(selection);
}

static uint64_t sys_data_address_wr(uint64_t a, uint64_t b, uint64_t c) {
	return sys_data_dir();
}

static uint64_t sys_time_wr(uint64_t selection, uint64_t unused1, uint64_t unused2) {
	return sys_time(selection);
}

static uint64_t sys_clear_wr(uint64_t unused1, uint64_t unused2, uint64_t unused3) {
	return sys_clear();
}
/* WRAPPERS */

/* Ejecuta la system call correspondiente al valor de rax */
uint64_t syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx) {
	if (rax < SYSCALL_S && rax >= 3)
		return (*syscalls[rax])(rbx,rcx,rdx);
	return 0;
}

/* SystemCall de Read para leer de entrada estandar*/
uint64_t sys_read(uint64_t fds, char * buffer, uint64_t bytes) {
	unsigned int i = 0;
	char c;
	int block=-1;
			c = poll_keyboard_buffer(buffer,bytes);
			if (c != '\0') {
				//*buffer = c;
				//printChar(*buffer);
				block=0;
			} 
			else{
				block= -1;
			}

		// else {
		// 	_hlt(); <--- en asm
		// }
    return block;
}

/* SystemCall de Write para escribir a salida estándar */
uint64_t sys_write(uint64_t fds, const char  str, uint64_t length) {
	if (fds == STDERR) { // podriamos darle un color especial para diferenciar los errores
		printString(str);
	} 
	else if (fds == STDOUT) {
		// if(*str == '\n' || *str == '\b'){
		// 	printString("bhello");
		// 	return 0;
		// }
		printChar(str);
	}
	return 0;
}


/* SystemCall Malloc */
uint64_t sys_malloc(uint64_t bytes) {
	//return (uint64_t) malloc(bytes);
}

/* System call que retorna la dirección del módulo de datos */
uint64_t sys_data_dir() {
	return DATA_DIR;
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

uint64_t sys_clear(){
	clear();
	return 0;
}
