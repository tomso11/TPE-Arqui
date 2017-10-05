#include "queueADT.h"
#include "memoryAllocator.h"

typedef struct node {
  qelem_t elem;
  struct node * next;
} node;

struct queue {
  node * first;
  node * last;
};

queueADT create_queue() {
  queueADT q = (queueADT) get_page(sizeof(*q));
  if (q == NULL)
    return NULL;
  q->first = NULL;
  q->last = NULL;
  return q;
}

void destroy_queue(queueADT q) {
  while(!is_empty(q))
    dequeue(q);

  store_page((uint64_t) q);
}

int is_empty(queueADT q) {
  if (q == NULL || q->first == NULL)
    return 1;
  else
    return 0;
}

void enqueue(queueADT q, qelem_t elem) {
  if (q == NULL)
    return;
  node * new_node = (node *) get_page(sizeof(*new_node));
  new_node->elem = elem;
  new_node->next = NULL;

  if (q->first == NULL) {
    q->first = new_node;
    q->last = new_node;
  }
  else {
    q->last->next = new_node;
    q->last = new_node;
  }
}

qelem_t peek(queueADT q) {
  if (is_empty(q))
    return NULL;
  return q->first->elem;
}

qelem_t dequeue(queueADT q) {
  qelem_t elem;
  if (is_empty(q))
    return NULL;
  node * f = q->first;
  elem = f->elem;
  q->first = q->first->next;

  if (q->first == NULL)
    q->last = NULL;

  store_page((uint64_t)f);
  return elem;
}
