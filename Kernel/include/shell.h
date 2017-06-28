#ifndef _SHELL_H_
#define _SHELL_H_


#define BUFF_SIZE 256
#define OK 0
#define NOT_EXIST 1
#define ERROR_ARGS 2
#define MAX_ARGS 3
#define MAX_ARG_LEN 15

typedef struct {
	char * command;  /* Nombre del comando */
	char** args;
	int args_num;
} command_t;

#include <stdio.h>
#include <stdint.h>

int shell();

void echon(char * buffer, int index);

int parsebuffer(char * buffer);

void bufferClean (char * buffer);

#endif
