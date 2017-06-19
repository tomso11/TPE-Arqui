#ifndef _SHELLCOMMANDS_H_
#define _SHELLCOMMANDS_H_

#define OK 0
#define NOT_EXIST 1
#define ERROR_ARGS 2
#define COMMAND_AMOUNT (sizeof(commands)/sizeof(commands[0]))

#include <stringlib.h>
#include "shell.h"

typedef struct {
	const char * command;  /* Nombre del comando */
	int (*function) (char ** args, int args_num);  /* Funcion correspondiente al comando */
} commandstruct;

int executecommand(const char *command, const char **args);
int execute_cmd(command_t cmd);
static int help(const char ** args, int args_num);
static int currentTime(const char **args, int args_num);
static void printTime(unsigned int n);
static int clear_sc(const char **args, int args_num) ;
static int echo(const char ** args, int args_num);

#endif