#include "processManager.h"
#include "kernelMutex.h"
#include "strings.h"
#include "process.h"
#include "queueADT.h"
#include "ipc_info.h"

typedef struct {
  char name[MUTEX_NAME_LEN];
  int64_t locked;
  int64_t state;
  queueADT process_queue;
} mutex;


static mutex open_mutexes[MAX_MUTEXES];
static int64_t array_mutex = UNLOCKED;

extern int _unlocked(int64_t * locknum);

static process * dequeue_process(mutex *m);
static void queue_process(mutex *m, process * p);

static mutex create_new_mutex(char * name);
static int is_open(int key);

static void lock_array();
static void unlock_array();

static void fill_mutex_info(mutex_info * m_info, int mutex_key);

static int is_open(int key) {
  return key >= 0 && key < MAX_MUTEXES && open_mutexes[key].state == OPEN;
}

static void lock_array() {
  while (!_unlocked(&array_mutex))
    yield_process();
}

static void unlock_array() {
  array_mutex = UNLOCKED;
}

int mutex_open(char * name) {
  int k;

  lock_array();

  for (k = 0; k < MAX_MUTEXES && is_open(k); k++) {
    if (strcmp(name, open_mutexes[k].name) == 0) {
      unlock_array();
      return k;
    }
  }

  if (k == MAX_MUTEXES) {
    unlock_array();
    return MAX_MUTEX_OPEN_ERROR;
  }

  open_mutexes[k] = create_new_mutex(name);

  unlock_array();

  return k;
}

static mutex create_new_mutex(char * name) {
  mutex m;
  strcpy(m.name, name);
  m.process_queue = create_queue();
  m.state = OPEN;
  m.locked = UNLOCKED;
  return m;
}

int mutex_close(int key) {
  lock_array();

  if (is_open(key)) {

    mutex * m = &open_mutexes[key];

    while (!is_empty(m->process_queue)) // Hay procesos lockeados
      unblock_process(dequeue_process(m));

    m->state = CLOSED;
    destroy_queue(m->process_queue);

    unlock_array();
    return 1;
  }

  unlock_array();
  return NOT_OPEN_ERROR;
}

int mutex_lock(int key) {
  if (is_open(key)) {

    set_superlock();

    mutex *m = &open_mutexes[key];
    process * p = get_current_process();

    if (!_unlocked(&m->locked)) {

    /* Encolar y bloquear debe ser atomico.
    ** Si se encola y no bloquea, puede que se desencole y desbloquee y luego bloquearse.
    ** Si se bloquea y no encola, como esta bloqueado no se encolara, entonces no se desencolara y nunca desbloqueara.
    ** El superlock asegura que el proceso actual volvera a correr dada una interrupcion de timer tick. */

      queue_process(m, p);

      block_process(p);

      unset_superlock();
      yield_process();
    }
    else
      unset_superlock();

    return 1;
  }
  return NOT_OPEN_ERROR;
}

int mutex_unlock(int key) {
  if (is_open(key)) {

    set_superlock();

    mutex *m = &open_mutexes[key];

    process * p = dequeue_process(m);

    while (is_delete_process(p))
      p = dequeue_process(m);

    m->locked = UNLOCKED;

    if (p != NULL) {
      unblock_process(p);
      m->locked = LOCKED;
    }

    unset_superlock();

    return 1;
  }
  return NOT_OPEN_ERROR;
}

int get_mutexes_info(mutex_info info_array[]) {
  int i, j;

  lock_array();

  for (i = j = 0; i < MAX_MUTEXES; i++) {
    if (is_open(i))
      fill_mutex_info(&info_array[j++], i);
  }

  unlock_array();

  return j;
}

static void fill_mutex_info(mutex_info * m_info, int mutex_key) {
  strcpy(m_info->name, open_mutexes[mutex_key].name);
  m_info->locked = open_mutexes[mutex_key].locked;
  m_info->key = mutex_key;
}

static void queue_process(mutex *m, process * p) {
  enqueue(m->process_queue, p);
}

static process * dequeue_process(mutex *m) {
  return dequeue(m->process_queue);
}
