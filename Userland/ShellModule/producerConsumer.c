#include "producerConsumer.h"
#include "stdio.h"
#include "time.h"
#include "shellcommands.h"
#include "rand.h"
#include "ctype.h"
static void producer();
static void consumer();
static void control_speed();
static void print_speeds();
static void print_commands();

static int producerSleep = INITIAL_PROD_SLEEP;
static int consumerSleep = INITIAL_CONS_SLEEP;

void start_producer_consumer_problem(int buf_size) {
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);
  int prod_pid, cons_pid;
  int i;
  char empty = EMPTY_SLOT;

  /* Llena el fifo de slots vacíos con cantidad de mensajes igual al tamaño del buffer */
  for (i = 0; i < buf_size; i++)
    write(empty_fd, &empty, 1); //Tenemos que usar un syscall diferente ya que este solo escribe en pantalla

  print_commands();

  prod_pid = exec_proc(producer, "producer");
  cons_pid = exec_proc(consumer, "consumer");

  control_speed();
  kill(prod_pid);
  kill(cons_pid);
  fifo_close(empty_fd);
  fifo_close(full_fd);
}

static void producer() {
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  char message;

  /* Lee espacios vacíos en empty_fd y escribe ítems en full_fd */
  while(1) {
    read(empty_fd,  &message, 1); // TBI una nueva syscall que nos permita leer de un fd
    int item = rand_int();
    printf("Produced: %d\n", item);
    sleep(producerSleep * SLEEP_MULTIPLIER);
    write(full_fd, &item, sizeof(int));
  }
}

static void consumer() {
  int empty_fd = fifo_open(PRODUCER_FIFO);
  int full_fd = fifo_open(CONSUMER_FIFO);

  int item;
  char message = EMPTY_SLOT;

  /* Lee ítems en full_fd y escribe slots vacíos en empt_fd */
  while(1) {
    read(full_fd,  &item, sizeof(int)); // lee el valor a una variable ?
    printf("Consumed: %d\n", item);
    sleep(consumerSleep * SLEEP_MULTIPLIER);
    write(empty_fd, &message, 1);
  }
}

static void control_speed() {
  char c;
  while((c = getchar())) {
    if (!isspace(c)) {
      switch (c) {
        case 'h':
          print_commands();
          break;
        case 'a':
          producerSleep++;
          print_speeds();
        break;
        case 's':
          producerSleep = producerSleep > 0 ? producerSleep-1 : 0;
          print_speeds();
        break;
        case 'z':
          consumerSleep++;
          print_speeds();
        break;
        case 'x':
          consumerSleep = consumerSleep > 0 ? consumerSleep-1 : 0;
          print_speeds();
        break;
        case 'q':
          getchar(); /* Limpia el \n que queda en el buffer */
          return;
        break;
        default:
          printf("Invalid command %c\n", c);
          print_commands();
          break;
      }
    }
  }
}

static void print_speeds() {
  printf("Producer sleep: %d\n", producerSleep);
  printf("Consumer sleep: %d\n", consumerSleep);
}

static void print_commands() {
  printf("\nCommands\n");
  printf("h   Prints this help.\n");
  printf("a   Increases producer sleep.\n");
  printf("s   Decreases producer sleep.\n");
  printf("z   Increases consumer sleep.\n");
  printf("x   Decreases consumer sleep.\n");
  printf("q   Ends producer consumer problem.\n");
  putchar('\n');
}
