#ifndef _SHELLCOMMANDS_H_
#define _SHELLCOMMANDS_H_

#define OK 0
#define NOT_EXIST 1
#define ERROR_ARGS 2
#define COMMAND_AMOUNT (sizeof(commands)/sizeof(commands[0]))

#include <stringlib.h>

typedef struct {
	const char * command;  /* Nombre del comando */
	int (*function) (const char *str);  /* Funcion correspondiente al comando */
} commandstruct;

int executecommand(const char *command, const char *args);
static int help(const char *args);
static int currentTime(const char *args);
static void printTime(unsigned int n);
static int clear(const char *args);
static int echo(const char *args);

#endif