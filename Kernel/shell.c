#include "stdio.h"
#include "shellcommands.h"
#include "shell.h"

#include "driverVideo.h" //debug

//static int buff_idx=0; //variable para recorrer el buffer que se reinicia en cada loop

typedef struct {
	char * arg;  /* Nombre del comando */
	int buff_idx;
} arg_t;


int shell(){
	int loop=1;
	char buff[BUFF_SIZE];
	int commandlen;
	char command[BUFF_SIZE];
	int hasarguments=0;
	int error;
	// char ** arguments;
	// arguments=my_malloc(sizeof(char*)*MAX_ARGS);
	// for(int j=0; j<MAX_ARGS; j++){
	// 	arguments[j]=my_malloc(sizeof(char)*MAX_ARG_LEN);
	// }
	// ncPrintDec(sizeof(arguments));
	// ncPrintDec(sizeof(arguments[0]));
	arg_t arg1;
	arg_t arg2;
	arg_t arg3;
	int i=0;
	char c;
	int s;
	command_t cmd;
	int continue_parse=0;
	int argus=0;
	int x;
	int* tracker=my_malloc(sizeof(int*));
	*tracker=0;

	

	while(loop ){
		//putstring("[User@gatOS]$  ");
		putchar('>');

		//printf("%s", "[User@gatOS]$  ");
		//while ( (getchar( (buff+i) )) != '\n'){
		while( (s=super_getchar()) != '\n' ){
			//printChar(( char)s);
			if(i == BUFF_SIZE){
				i = 0;
			}
			buff[i]=s;
			printChar(s);
			//putchar(s);
			i++;
				if(i > 0)
					//printChar(buff[i-1]);
			
			if( c != -1 ){
			}
			// ncPrintDec(c);
			// ncPrintChar(' ');
			// putchar(c);
			// putchar(c);
		}
		putchar('\n');
		buff[i]='\0';
		//printString("does it stop");


		// putchar('\n');
		
		// printString(cmd.command);
		// error = execute_cmd(cmd);
		// printErrorMsg(error);
		// buff[0]='\0'; //reinicio el buffer
		// i=0;

		space_eater(buff);
		(*tracker)=parsero(buff,cmd.command);
		// arg1.buff_idx=*tracker;

		// for(x=0; x<MAX_ARGS && *tracker != 0; x++){ // DIRTY
		// 	ncPrintDec(*tracker);
		// 	if(x==0){
		// 		arg1.arg=arg_parser(buff, tracker);
		// 		ncPrintDec(arg1.buff_idx);
		// 	}
		// 	if(x==1){
		// 		arg2.arg=arg_parser(buff, tracker);
		// 		arg2.buff_idx=*tracker;
		// 		ncPrintDec(arg2.buff_idx);
		// 	}
		// 	if(x==2){
		// 		arg3.arg=arg_parser(buff, tracker);
		// 		arg3.buff_idx=*tracker;
		// 		ncPrintDec(arg3.buff_idx);
		// 	}

		// }

		// while(continue_parse != 0){
		// 	printString("continues");
		// 	continue_parse=parsero(buff,arguments[argus],continue_parse);
		// 	printString(arguments[argus]);
		// 	argus++;
		// }

		//cmd=parser(buff);
		// printChar('\n'); // SUPER DEBUGGING BLOCK
		// printString(buff);
		// printChar('\n');
		// printString(cmd.command);
		// printChar('\n');
		// printString(arg1.arg);
		// printChar('\n');
		// printString(arg2.arg);
		// printChar('\n');
		// printString(arg3.arg);
		// printChar('\n');
		
		bufferClean(buff);
		i=0;
		(*tracker)=0;


	}
	// while(loop){
	// 	printString("[User@gatOS]$ ");
	// 	readRowAndClear(buff, len);
	// 	if(buff[0] != '\0'){
	// 		commandlen = interpret(buff, command);
	// 		if(buff[commandlen] != '\0'){
	// 			hasarguments = 1 ;
	// 		}	
	// 		error = executeCommand(command, buff+commandlen+hasarguments);
	// 		printErrorMsg(error);
	// 	}
	// }

	return 0;
}

/*DEPRECATED*/

static int interpret(const char * buffer, char * command){
	int i;
	for (i=0; buffer[i]!='\0' && buffer[i]!=' '; i++){
		command[i]=buffer[i];
	}
	command[i]= '\0';
	return i;
}

static void printErrorMsg(int error){
	switch(error){
		case ERROR_ARGS:
			//printf("%s", "Invalid arguments \n");
			putstring("Invalid args \n");
			break;
		case NOT_EXIST:
			//printf("%s", "Command does not exist" );
			putstring("Command does not exist\n");
			break;
	}

}

/*DEPRECATED*/
/* come espacios y aplica los backspace */
static void cleanBuffer(char* buffer){
	int i;
	int j;
	for (i=0; buffer[i] != '\0' ; i++){
		if( buffer[i] == '\b' ){
			if(i != 0){
				for(j=i; buffer[j+1] != '\0' ; j++ ){ // revisar para eficiencia
					buffer[j]=buffer[j+2];
				}
				buffer[j]='\0';
			}
			else{
				buffer[i]='\0';
				return;
			}
		}
		while( buffer[i] == ' ' ){ // come espacios
			buffer[i]=buffer[i+1];
		}
	}
}

/* Toma el input y separa el comando y los argumentos .*/
static command_t parseCmd( const char * buffer ){
	int i;
	int j=0;
	int n=0;
	command_t cmd;
	for ( i=0; buffer[i] != '\0'; i++ ){ //copia el command
		if(buffer[i] == ' '){ //separa cmd de args
			i++; //salteo el espacio
			while(buffer[i] != '\0'){ //copia los args
				if(buffer [i] == ' '){ //separa los args
					cmd.args[n][j]='\0';
					n++;
					j=0;
				}
				cmd.args[n][j++]=buffer[i++];
			}
			cmd.args[n][j]=buffer[i];
		}
		cmd.command[i]=buffer[i];
		if(cmd.command[i] == '\0'){ //puede que salga del while anidado con un '\0'
			i--;
		}
	}
	//cmd.command
	cmd.args_num=n;
	return cmd;
}

/*Borra los espacios repetidos (pretends to be shocked)*/

static void space_eater(char* buffer){
	int j;
	int aux;
	for(j=0; buffer[j] != '\0'; j++){
		if( (buffer[j] == ' ' && buffer[j+1] == ' ') || (buffer[j]== ' ' && j==0) ){
			for(aux=j; buffer[aux] != '\0'; aux++){
				buffer[aux]=buffer[aux+1];
			}
			j--;
		}
	}
}

/* re implementacion del parser */

static command_t parser(const char* buffer){
	int n=0;
	command_t cmd={{0, 0, 0}};
	command_t aux;
	for (n = 0; buffer[n] != ' ' && buffer[n] != '\0' ; n++)
	{
		cmd.command[n]=buffer[n];
	}
	cmd.command[n]=buffer[n];
	if(buffer[n] == ' '){
		aux=parse_args(buffer, n);
		cmd.args=aux.args;
		cmd.args_num=aux.args_num;
	}
	return cmd;
}

static command_t parse_args(const char* buffer, int n){
	command_t ans;
	int arg_num=1;
	int arg_idx=0;
	n++; // salteo el espacio con el que llega
	while(buffer[n] != '\0'){ //hasta terminar el input
		ans.args=my_malloc((sizeof(char*) * 3)); //agrabdamos cada vez que hay un nuevo argumento
		ans.args[arg_num-1]=my_malloc(sizeof(char)); // 15 monto arbitrario
		ncPrintDec(sizeof(ans.args));
		putchar('\n');
		ncPrintDec(sizeof(ans.args[0]));
		putchar('\n');
		ncPrintDec(sizeof(char*));
		putchar('\n');
		ncPrintDec(sizeof(char));
		for( ; buffer[n] != ' ' || buffer[n] != '\0'; n++){ //copiamos el arg
			ans.args[arg_num-1][arg_idx++]=buffer[n];
		}
		ans.args[arg_num-1][arg_idx]='\0'; //corto el arg
		if(buffer[n] != '\0'){
			arg_num++; //agregamos otro argumento
			arg_idx=0;
		}
	}
	ans.args_num=arg_num;
	return ans;
}

/* intento de parser que retorna 1 si todavia hay comandos o 0 si el buffer corto*/
static int parsero(const char* buffer, char* tgt){
	int i=0;
	int n=0;
	for (; buffer[n] != ' ' && buffer[n] != '\0' ; n++)
	{
		tgt[i++]=buffer[n];
	}
	tgt[i]='\0';
	if(buffer[n] == ' '){
		return n+1;
	}
	return 0;
}

static char* arg_parser(const char* buffer, int * tracker){
	char *aux=my_malloc(sizeof(char)*MAX_ARG_LEN);
	int a_idx=0;
	int n=*tracker;
	for ( ; buffer[n] != ' ' && buffer[n] != '\0' ; n++)
	{
		aux[a_idx]=buffer[n];
	}
	if(buffer[n] == ' '){
		n++;
	}

	printString("en arg_psr");
	ncPrintDec(n);
	*tracker=n;
	aux[a_idx]='\0';
	for(a_idx=0; aux[a_idx] != '\0'; a_idx++){
		printChar(aux[a_idx]);
	}
	return aux;
}

static void bufferClean( char* buffer){
	int n;
	for(n=0; buffer[n] != '\0'; n++){
		buffer[n]='\0';
	}
}