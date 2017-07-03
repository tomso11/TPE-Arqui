#include "stdio.h"
#include "shellcommands.h"
#include "shell.h"
#include "stringlib.h"

//#include "driverVideo.h" //debug

//static int buff_idx=0; //variable para recorrer el buffer que se reinicia en cada loop

#define NAME_MAX 11



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
		case OK:
			break;
	}

}




int shell(){

	int loop = 1;
	char buffer[BUFF_SIZE];
	int index=0;
	int error=0;
	char c;
	int first=1;
	
	while(loop){
		
		putchar('>');

		while( (c=super_getchar()) != '\n' ){

			if(index==BUFF_SIZE){
				index=0;
			}

			if(c=='\b'){
				if(index!=0){
    				index--;
    				buffer[index]='\0';
				//backspace();

    				
    			}
    			} else {
    				buffer[index]=c;
    				index++;
				putchar(c);
				

		  		}
		

		}

		buffer[index]='\0';
		error = parsebuffer(buffer);
		putchar('\n');		
		printErrorMsg(error);
		putchar('\n');
		index=0;	
		bufferClean(buffer);
		error=OK;
		


		
	}

	return 0;
}

int main (){
	shell();
	return 0;
}

int parsebuffer(char * buffer){
	char commando[NAME_MAX]; //el name max depende de cuales sean las funciones que usemos. Por ejemplo: echo, clear, currentTime, help - name max = 11
	int index;

	


	for (index=0; buffer[index]!= ' ' && buffer[index]!='\0'; index++ ){
		commando[index]=buffer[index];
		if(index > NAME_MAX-1){
			bufferClean(commando);
			return NOT_EXIST; // por la funcion de printErrorMsg 		
		}
	}

	if (comp_str(commando, "clear")){
		if (buffer[index]!='\0'){
			bufferClean(commando);
			return ERROR_ARGS;
			
		}
		return usr_clear();
	
	}else if(comp_str(commando, "echo")){
		putchar('\n');
		echon(buffer, index+1);
	
	} else if (comp_str(commando, "help")){
		if (buffer[index]!='\0'){
			bufferClean(commando);
			return ERROR_ARGS;
			
		}
		putchar('\n');
		putstring("help - muestra los comandos disponibles");		
		putchar('\n');
		putstring("echo - imprime en pantalla todos los argumentos que reciba");
		putchar('\n');
		putstring("clear - limpia la pantalla"); 
			

	}else if(comp_str(commando, "currentTime")){
		// ejecutar currentTime();
	}else{
		
		bufferClean(commando);
		return NOT_EXIST;
		
	}

	//si un comando va a tener argumentos del estilo %s, imitar la estructura de esta funcion e ir encadenando. Aca es donde se manejaria el ERROR_ARGS
	bufferClean(commando);
	return OK;
	
}

void echon(char * buffer, int index){

	int i;
	for (i=index; buffer[i]!= '\0'; i++){
		putchar(buffer[i]);
	}

}

void bufferClean(char * buffer){
	int n;
	for (n=0; buffer[n]!='\0'; n++){
		buffer[n]='\0';
	}	
}
