#ifndef _PROD_CON_H_
#define _PROD_CON_H_

#define PRODUCER_FIFO "Producer_fifo"
#define CONSUMER_FIFO "Consumer_fifo"
#define FULL_SLOT 'F'
#define EMPTY_SLOT 'E'

void start_producer_consumer_problem(int buf_size);

#define INITIAL_CONS_SLEEP 7
#define INITIAL_PROD_SLEEP 4

#define SLEEP_MULTIPLIER 300

#endif
