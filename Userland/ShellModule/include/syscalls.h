#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

uint64_t _int80h(uint64_t rax, uint64_t rbx, uint64_t rcx, uint64_t rdx);

int readC(char * buffer, int size); //en stdio.asm
void writeC(char * msg, int size);
void clearC();
void mallocC(int bytes);
int execC(void * fun_ptr, uint64_t memory, const char * name );


int read(unsigned int fd, char *buff, unsigned int bytes);
int write(unsigned int fd, char buff, unsigned int bytes);
void* malloc (unsigned int bytes);
int exec(void * fun_ptr, uint64_t memory, const char * name );
void * res (unsigned int bytes);
void * data_dir();
int time(int selector);
void clear_screen();

#endif
