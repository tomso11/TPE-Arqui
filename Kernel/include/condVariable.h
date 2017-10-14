#ifndef COND_VARIABLE_H
#define COND_VARIABLE_H

#define MAX_COND_VAR 128

#define COND_NAME_LEN 64

#define NOT_OPEN_ERROR -1
#define MAX_COND_OPEN_ERROR -2

#define OPEN 1
#define CLOSED 0

int initialize_conditional_variable();

int cond_open(char * name);
int cond_wait(int cond_key, int lock_key);
int cond_signal(int key);
int cond_broadcast(int key);
int cond_close(int key);

#endif
