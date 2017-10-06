#ifndef _QUEUE_H_
#define _QUEUE_H_

typedef void * qelem_t;

typedef struct queue * queueADT;

queueADT create_queue();

void destroy_queue(queueADT q);

int is_empty(queueADT q);

void enqueue(queueADT q, qelem_t elem);

qelem_t dequeue(queueADT q);

qelem_t peek(queueADT q);

#endif
