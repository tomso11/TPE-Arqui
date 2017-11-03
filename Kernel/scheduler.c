#include "memoryAllocator.h"
#include "scheduler.h"
#include "dirs.h"
#include "driverVideo.h"
#include "process.h"
#include "interruptions.h"

#include "naiveConsole.h"

#define QUANTUM 1

typedef struct c_node {
	int quantum;
	process *p;
	struct c_node *next;
} list_node;


static void add_process(process *p);
static void set_next_current();

void _change_process(uint64_t rsp);

extern int _unlocked(int64_t * locknum);

/* Proceso actualmente corriendo */
static list_node *current = NULL;
static list_node *prev = NULL;

void _yield_process();

/* Nos permite evitar un preempt del proceso */
static int64_t superlock = UNLOCKED;

void set_superlock() {
	superlock = LOCKED;
}

void unset_superlock() {
	superlock = UNLOCKED;
}

process * get_current_process() {
	return current->p;
}

uint64_t next_process(uint64_t current_rsp) {
	if (current == NULL)
		return current_rsp;

	unassign_quantum();

	if (current->quantum > 0)
		return current_rsp;

	if (!_unlocked(&superlock))
		return current_rsp;

	current->quantum = QUANTUM;

	set_rsp_process(current->p, current_rsp);

	prev = current;
	current = current->next;

	set_next_current();

	unset_superlock();

	return get_rsp_process(current->p);
}

uint64_t exec_process(process * new_process) {
	int pid;

	printString("Executing process ...");
	printString("\n");
	printString("+ Process name= ");
	printString(new_process->name);
	printString("\n");
	printString("+ Stack page= ");
	ncPrintDec(new_process->stack_page);
	printString("\n");
	printString("+ RIP= ");
	ncPrintDec(new_process->entry_point);
	printString("\n");
	printString("+ RSP= ");
	ncPrintDec(new_process->rsp);
	printString("\n");
	add_process(new_process);

	pid = pid_process(new_process);

	if (pid == 0) /*Si es init, cambiamos el sp al de init */
		_change_process(get_rsp_process(current->p));


	return pid;
}

static void add_process(process * p) {

	list_node *new_node = (list_node *) get_page(sizeof(*new_node));

	new_node->p = p;
	new_node->quantum = QUANTUM;

	set_superlock();

	if (current == NULL) {
		current = new_node;
		current->next = current;
		prev = current;
	}
	else {
		new_node->next = current->next;
		current->next = new_node;
	}

	unset_superlock();
}

void yield_process() {
	current->next->quantum += 1; /* Quantum al siguiente proceso pues el actual quitó tiempo */
	current->quantum = 0;
	_yield_process();
}

/* Se avanza con el proceso que está delante */
void end_process() {

	set_superlock();

	list_node * n = current;

	destroy_process(n->p);

	prev->next = current->next;

	current = current->next;

	set_next_current();

	unset_superlock();

	store_page((uint64_t) n);

	assign_quantum();  /* Se le da un quantum al nuevo proceso */

	_change_process(get_rsp_process(current->p));
}

static void set_next_current() {
	while (is_blocked_process(current->p) || is_delete_process(current->p)) {
		list_node * next = current->next;

		if (is_delete_process(current->p)) {
			prev->next = next;
			destroy_process(current->p);
			store_page((uint64_t) current);
		}
		else
			prev = current;

		current = next;
	}
}

void assign_quantum() {
	current->quantum += 1;
}

void unassign_quantum() {
	current->quantum -= 1;
}
