#include "philo.h"
#include "time.h"
#include "stdio.h"
#include "syscalls.h"
#include "strings.h"
#include "random.h"
#include "executer.h"
#include "stdlib.h"
#include "ctype.h"

#define PHILO_PROCESS_NAME "philosopher"

int critical_m;
int philosopherCount = 0;
int modify_cond_var;
int finished_eating_cond;

static philosopher_data philo_array [MAX_PHILOSOPHERS];

static int pause = UNPAUSED;

static void take_forks(int i);
static void put_forks(int i);
static void test(int i);
static int philosopher(int argc, char * argv[]);
static void setState(int philo, int st);

static int add_philosopher();
static void remove_philosopher();
static void remove_all_philosophers();
static void release_resources();
static void slow_die();

static void pause_philosophers();
static int is_paused();

static int should_die(int i);

/* Se puede cambiar para imprimir de distinta forma los filósofos. En
** este caso se cambia entre modo texto y modo gráfico */
static void (* render) (philosopher_data * philo_array, int philosopherCount);

void listen_commands();

int start_philosophers_problem(int philoNumber) {
  int i;
  philoNumber = philoNumber > MAX_PHILOSOPHERS ? MAX_PHILOSOPHERS : philoNumber;

  render = renderText;

  critical_m = mutex_open(MAIN_MUTEX_NAME);
  modify_cond_var = cond_open(MODIFY_COND_NAME);
  srand(seconds() * minutes() * hour());

  philosopherCount = 0;

  print_commands();

  for (i = 0; i < philoNumber; i++) {
    add_philosopher();
  }

  listen_commands();
  getchar();
  return 0;
}

void listen_commands() {
  char c;
  while((c = getchar())) {
    if(!isspace(c)) {
      switch (c) {
        case 'h':
          print_commands();
          break;
        case 'e':
        if (!is_paused()) {
          remove_all_philosophers();
          return;
        }
        break;
        case 'w':
        if (!is_paused())
          add_philosopher();
        break;
        case 's':
        if (!is_paused())
          remove_philosopher();
        break;
        case 'p':
          pause_philosophers();
        break;
        case 'q':
        if (!is_paused()) {
          slow_die();
          return;
        }
        break;
        default:
          printf("Invalid command %c\n", c);
          print_commands();
          break;
      }
    }
  }
}

/* Bloquea el mismo mutex que usan los filósofos para que se queden pausados */
static void pause_philosophers() {
  if (!is_paused()) {
    pause = PAUSED;
    mutex_lock(critical_m);
    printf("PAUSED\n");
  }
  else {
    pause = UNPAUSED;
    mutex_unlock(critical_m);
    printf("RESUMED\n");
  }
}

static int is_paused() {
  return pause == PAUSED;
}

static int add_philosopher() {
  char name[] = "PhilosopherMutex000";
  char args[3];
  int new_pid = -1, philo_index;

  mutex_lock(critical_m);
  if (philosopherCount < MAX_PHILOSOPHERS) {
    philo_index = philosopherCount;
    philosopherCount += 1;
    itoa(philo_index, name + strlen(name) - 3, 10);

    philosopher_data * pd = &philo_array[philo_index];
    pd->die = 0;
    pd->mut = mutex_open(name);

    pd->cond_die = cond_open(name);

    mutex_lock(pd->mut);

    pd->state = THINKING;
    itoa(philo_index, args, 10);

    new_pid = execp(philosopher, args, PHILO_PROCESS_NAME);
    pd->pid = new_pid;
  }
  mutex_unlock(critical_m);
  return new_pid;
}

static void remove_philosopher() {
  mutex_lock(critical_m);

  if (philosopherCount > 0) {
    int count = philosopherCount;
    int philo_index = philosopherCount - 1;
    philo_array[philo_index].die = 1;
    while (count == philosopherCount)
      cond_wait(modify_cond_var, critical_m);
  }

  mutex_unlock(critical_m);
}

/* Remueve un filósofo a la vez en lugar de hacerles kill */
static void slow_die() {
  int i;
  int count = philosopherCount;
  for (i = 0; i < count; i++) {
    remove_philosopher(i);
  }
  release_resources();
}

static void release_resources() {
  mutex_close(critical_m);
  cond_close(modify_cond_var);
}

/* Cierra todos los mutexes abiertos para el problema y le hace kill a
** todos los filósofos corriendo */
static void remove_all_philosophers() {
  mutex_lock(critical_m);
  int i, philoCountAux = philosopherCount;
  for (i = 0; i < philoCountAux; i++) {
    kill(philo_array[i].pid);
    mutex_close(philo_array[i].mut);
    cond_close(philo_array[i].cond_die);
  }
  mutex_unlock(critical_m);
  release_resources();
}

static int philosopher(int argc, char * argv[]) {
  int i = atoi(argv[0]);
  while(1) {
    sleep(rand_int_range(2, 7) * 1000);

    if (should_die(i)) {
      return 1;
    }

    take_forks(i);

    sleep(rand_int_range(2, 7) * 1000);
    put_forks(i);
  }
}

/* Si el filósofo fue marcado para morir, se lo pone en estado DYING, y espera
** a que alguno de los dos filósofos a su lado terminen de comer para
** efectivamente desaparecer */
static int should_die(int i) {
  int die;
  mutex_lock(critical_m);
  philosopher_data * pd = &philo_array[i];
  die = pd->die;

  if (die) {
    setState(i, DYING);
    while(philo_array[LEFT(i, philosopherCount)].state == EATING \
      && philo_array[RIGHT(i, philosopherCount)].state == EATING) {
        cond_wait(pd->cond_die, critical_m);
    }
    pd->die = 0;
    philosopherCount -= 1;
    mutex_close(pd->mut);
    cond_close(pd->cond_die);
    //render(philo_array, philosopherCount);
    cond_signal(modify_cond_var);
  }

  mutex_unlock(critical_m);

  return die;
}

static void take_forks(int i) {
  mutex_lock(critical_m);
  setState(i, HUNGRY);
  test(i);
  mutex_unlock(critical_m);
  mutex_lock(philo_array[i].mut);
}

static void put_forks(int i) {
  mutex_lock(critical_m);

  setState(i, THINKING);
  test(LEFT(i, philosopherCount));
  cond_signal(philo_array[LEFT(i, philosopherCount)].cond_die);

  test(RIGHT(i, philosopherCount));
  cond_signal(philo_array[RIGHT(i, philosopherCount)].cond_die);

  mutex_unlock(critical_m);
}

/* Además de settear el estado, imprime en pantalla el estado de los filósofos */
static void setState(int philo, int st) {
  philo_array[philo].state = st;
  render(philo_array, philosopherCount);
}

static void test(int i) {
  if (philo_array[i].state == HUNGRY \
    && philo_array[LEFT(i, philosopherCount)].state != EATING \
      && philo_array[RIGHT(i, philosopherCount)].state != EATING) {
    setState(i, EATING);
    mutex_unlock(philo_array[i].mut);
  }
}
