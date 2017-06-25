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

static char* arg_parser(const char* buffer, int* trckr);

static int parsero(const char* buffer, char* tgt);

static command_t parse_args(const char* buffer, int n);

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
