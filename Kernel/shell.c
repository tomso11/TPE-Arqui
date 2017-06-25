#include "stdio.h"
#include "shellcommands.h"
#include "shell.h"

#include "driverVideo.h" //debug


int shell(){
	int loop=1;
	char buff[BUFF_SIZE];
	int commandlen;
	char command[BUFF_SIZE];
	int hasarguments=0;
	int error;

	int i=0;
	char c;
	int s;
	command_t cmd;
	

	//
	//load en pantalla ?
	//

	/*PUT STRING HACE EXPLOTAR TODOOOOO*/
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
		buff[i]='\0';
		//printString("does it stop");


		// putchar('\n');
		
		// printString(cmd.command);
		// error = execute_cmd(cmd);
		// printErrorMsg(error);
		// buff[0]='\0'; //reinicio el buffer
		// i=0;

		space_eater(buff);
		//cmd = parseCmd(buff);
		cmd=parser(buff);
		printChar('\n'); // SUPER DEBUGGING BLOCK
		printString(buff);
		printChar('\n');
		printString(cmd.command);
		printChar('\n');
		bufferClean(buff);
		i=0;


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
	for (n = 0; buffer[n] != ' ' && buffer[n] != '\0' ; n++)
	{
		cmd.command[n]=buffer[n];
	}
	cmd.command[n]=buffer[n];
	return cmd;
}

static void bufferClean( char* buffer){
	int n;
	for(n=0; buffer[n] != '\0'; n++){
		buffer[n]='\0';
	}
}