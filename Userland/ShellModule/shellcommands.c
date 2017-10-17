#include "stringlib.h"
#include "shellcommands.h"
#include "shell.h"

#define OK 0
#define NOT_EXIST 1
#define ERROR_ARGS 2
#define COMMAND_AMOUNT (sizeof(commands)/sizeof(commands[0]))
#define PAGE 0x1000


static commandstruct commands[]= {{"echo", echo},{"clear", clear_sc},{"time", currentTime}, {"help", help}//Agregar comandos aca
												};


/* DEPRECATED */
// int executecommand(const char *command, const char ** args){
// 	int i;
// 	for (i=0; i<COMMAND_AMOUNT; i++){
// 		if(strcmp(command, commands[i].command)==0){
// 			return (*commands[i].function)(args);
// 		}
// 	}
// 	return NOT_EXIST;
// }

int execute_cmd(command_t cmd){
  int n;
  int pid;
  int validity=0;
  for (n=0; n<COMMAND_AMOUNT; n++){
    if(comp_str(cmd.command, (commands[n].command) )==0){
      validity=1;
      pid = exec_proc(commands[n].function, cmd);
    }
  }
  return validity;
}


int exec_proc(void * function, command_t cmd){
  void * memory = malloc(PAGE);
  char ** arguments = memory;
  char * arg_strings = memory + MAX_ARGS * sizeof(char*);
  int pid;

  arguments[0]= (void *) function; // el primer argumento sera el nombre de la funcion

  //build_args(arg, arg_strings, arguments + 1); /* take a look */

  pid = exec(function, (uint64_t)memory, cmd.command); 

  //yield(); // una vez finalizado el proceso

  //free(memory);

  return pid;
}


static int help(const char ** args, int args_num){
  if (args_num != 0){
  	return ERROR_ARGS;
  } else{
    char ** msg=my_malloc(sizeof(char*));
    char * s="The available commands are the following:\nhelp: Display information about existing commands\necho[args...]: Display the arguments on screen\nclear:clear the screen\ntime: Display the current time on screen in 24hr. format[hh:mm:ss]\n";
    msg[0]=s;
    putstring(*msg);
  // 	printf("help: Display information about every existent command\n");
 	//   printf("echo [args...]: Display the arguments spaced on the screen\n");
	 // printf("clear:  Clear the screen.\n");
	 // printf("time: Display current time on the screen (24hr format [hh:mm:ss])\n");
  return OK;
	}
}

static int currentTime(const char **args, int args_num) {
  if(args_num != 0){
    return ERROR_ARGS;
  }
  int h = hour(), m = minutes(), s = seconds();
  printf("Current time: ");
  printTime(h);
  printf(":");
  printTime(m);
  printf(":");
  printTime(s);
  printf("\n");
  return OK;
}

/* Imprime un numero en formato [xx]*/
static void printTime(unsigned int n) {
    if (n < 10){
        printf("0");
    }    
    printf("%d", n);
}

static int clear_sc(const char **args, int args_num) {
  if(args_num != 0){
    return ERROR_ARGS;
  }
  return usr_clear();
}


static int echo(const char ** args, int args_num) {
  if(args_num == 0){
    return ERROR_ARGS;
  }
  int i;
  for(i=0; i< args_num ; i++){
    printf("%s\n", args[i]);  
  }
  
  return OK;
}




	 
  