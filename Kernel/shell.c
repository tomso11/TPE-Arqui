#include <terminal.h>
#include <driverVideo.h>

void terminal(void) {
	int exit = 0;
	char* command;
	int proc;

	do {
		printOnScreen("[User@gatOS]$ ");
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


		while(1);
	} while(!exit);
	return !exit;
}