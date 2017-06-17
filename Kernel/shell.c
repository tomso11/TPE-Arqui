#include <shell.h>
#include <driverVideo.h>

void terminal(void) {
	int exit = 0;
	char* command;
	int proc;

	do {
		printString("[User@gatOS]$ ");
		command=input_line();
		proc=process_start(command);
		while( process_executing(proc) ){
			if( process_waiting_input() ){
				char * line= input_line();
				process_send_input(proc, line);	
			}
			if( process_waiting_output() ){
				char * output=process_get_output();
				output_line(output);
				end(proc);
			}
		}


	} while(!exit);
	return !exit;
}

char* input_line(){
	return 0;
}
int process_start(char* cmd){
	return 0;
}
int process_executing(int proc){
	return 0;
}
int process_waiting_input(){
	return 0;

}
void process_send_input(int proc, char* line){
	return ;
}
int process_waiting_output(){
	return 0;
}
char* process_get_output(){
	return 0;
}
void output_line(){
	return;
}
void end(){
	return;
}
