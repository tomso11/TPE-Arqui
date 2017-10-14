#ifndef  _FIFO_H_
#define _FIFO_H_

#define MAX_FIFOS 128

#define FIFO_NAME_LEN 64
#define BUF_SIZE 1024

#define FIFO_NOT_OPEN_ERROR -1
#define MAX_FIFOS_OPEN_ERROR -2

#define OPEN 1
#define CLOSED 0

int fifo_open(char * name);

int fifo_close(int key);

int fifo_write(int key, const void * buf, int bytes);

int fifo_read(int key, void * buf, int bytes);

void initialize_fifo_mutex();

#endif
