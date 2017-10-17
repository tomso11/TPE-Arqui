#include "syscalls.h"
//#include "driverVideo.h" //debug

/* Ambas llamadas son llamadas desde "stdio.c" y llaman funciones de "stdio.asm" */

/* SystemCall Read */
int read(unsigned int fd, char *buff, unsigned int bytes) {
     int i;
     readC(buff,bytes);
     //i=_int80h(3, fd, (uint64_t) buff, bytes);
     //printChar(*buff);
     return i;


}

/* SystemCall Write */
int write(unsigned int fd, char buff, unsigned int bytes) {
    //return _int80h(4, fd, (uint64_t) buff, bytes);
    int i;
    writeC(buff,bytes);
    return i;
}

/* NOT IMPLEMENTED */
/* SystemCall para malloc*/
void* malloc (unsigned int bytes) {
    /*return (void*)*/ mallocC(bytes); // casteqar a direccion de una pagina
	//return (void *) _int80h(5, bytes, 0, 0);
}

int exec(void * fun_ptr, uint64_t memory, const char * name ){
    return execC(fun_ptr, memory, name);
}

/* SystemCall para saber direccion de memoria*/
void * data_dir() {
    //return (void *) _int80h(6, 0, 0, 0);
}

/* SystemCall Time */
int time(int selector) {
    //return _int80h(7, selector, 0, 0);
}
/* Syscall Clear Screen */
void clear_screen(){
    clearC();
	//return _int80h(8, 0, 0, 0);
}
