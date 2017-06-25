#ifndef _SHELL_H_
#define _SHELL_H_


#define BUFF_SIZE 256
#define OK 0
#define NOT_EXIST 1
#define ERROR_ARGS 2

typedef struct {
	char * command;  /* Nombre del comando */
	char** args;
	int args_num;
} command_t;

#include <stdio.h>
#include <stdint.h>

int shell();

static void bufferClean( char* buffer);

static void space_eater(char* buffer);

static command_t parser(const char*buffer);

static int interpret(const char * buffer, char * command);

static void printErrorMsg(int error);

/* come espacios y aplica los backspace */
static void cleanBuffer(char* buffer);

/* Toma el input y separa el comando y los argumentos .*/
static command_t parseCmd( const char * buffer );

#endif
