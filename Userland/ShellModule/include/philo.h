#ifndef _PHILO_H_
#define _PHILO_H_

#define LEFT(x, N) (((x)+(N)-1) % (N != 0 ? N : 1))
#define RIGHT(x, N) (((x)+1) % (N != 0 ? N : 1))
#define THINKING 0
#define HUNGRY 1
#define EATING 2
#define DYING 3

#define UNPAUSED 1
#define PAUSED 0

#define MAX_PHILOSOPHERS 16

#define MAIN_MUTEX_NAME "MAIN_PHILOSOPHERS_MUTEX"
#define MODIFY_COND_NAME "MODIFY_COND"


typedef struct philosopher_data {
  int state;
  int mut;
  int pid;
  int die;
  int cond_die;
} philosopher_data;

int start_philosophers_problem(int graphic, int philoNumber);

void renderText(philosopher_data * philos, int philosopherCount);

void renderGraphics(philosopher_data * philos, int philosopherCount);

void print_colors_instructions();

void print_commands();

#endif