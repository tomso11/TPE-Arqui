#ifndef _TERMINAL_H_
#define _TERMINAL_H_

void terminal(void);
char* input_line(void);
int process_start(char*);
int process_executing(int);
int process_waiting_input(void);
void process_send_input(int, char*);
int process_waiting_output(void);
char* process_get_output();
void output_line();
void end();

#endif