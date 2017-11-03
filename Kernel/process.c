#include "process.h"
#include "memoryAllocator.h"
#include "scheduler.h"
#include "timer.h"
#include "strings.h"
#include "mutex.h"

#include "naiveConsole.h"
#include "driverVideo.h"

#define MAX_PROCESS_NAME 64
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)));

/* El stack frame y el llenado del mismo se tomó de
** https://bitbucket.org/RowDaBoat/wyrm
*/
typedef struct {
	//Registers restore context
	uint64_t gs;
	uint64_t fs;
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;

	//iretq hook
	uint64_t rip;
	uint64_t cs;
	uint64_t eflags;
	uint64_t rsp;
	uint64_t ss;
	uint64_t base;
} StackFrame;

/* definido en header
struct c_process {
	status st;
	char name[MAX_PROCESS_NAME];
	uint64_t entry_point;
	uint64_t rsp;
	uint64_t stack_page;
	uint64_t n_data_page;
	void * data_page[MAX_DATA_PAGES];
	uint64_t pid;
	uint64_t ppid;
	uint64_t open_fds; 
};
*/


static process * process_table[MAX_PROCESSES] = {NULL};

int64_t _unlocked(int64_t * lock);

static void lock_table();

static void unlock_table();

static uint64_t fill_stack(uint64_t rip, uint64_t rsp, uint64_t params);

static void clean_data_page(process * p);

static void free_data_pages(process * p);

static void unblock_foreground_process(process * p);


static process * foreground = NULL;
static uint64_t n_processes = 0;
static uint64_t table_mutex_key = -1;


static void lock_table() {
  mutex_lock(table_mutex_key);
}

static void unlock_table() {
  mutex_unlock(table_mutex_key);
}

void initialize_process_mutex() {
	table_mutex_key = mutex_open("__PROCESS_TABLE_MUTEX__");
}


/* Inserta proceso en la tabla. Devuelve el pid del proceso y -1 en caso de error */
static int insert_process (process * p) {
	int i;

	lock_table();

	for (i = 0; i < MAX_PROCESSES; i++) {
		if (process_table[i] == NULL) {
			n_processes++;
			p->pid = i;
			process_table[i] = p;
			unlock_table();
			return i;
		}
	}

	unlock_table();

	return -1;
}

/* Adquiere los pid de los procesos corriendo. Termina con -1 y devuelve la longitud del arreglo. */
int get_current_pids(int pid_array[]) {
	int i, j, processes_left;

	lock_table();

	processes_left = n_processes;

	for (i = j = 0; i < MAX_PROCESSES && processes_left > 0; i++) {
		if (process_table[i] != NULL) {
			pid_array[j++] = pid_process(process_table[i]);
			processes_left--;
		}
	}

	pid_array[j] = -1;

	unlock_table();

	return j;
}

static void clean_data_page(process * p) {
	int i;

	for (i = 0; i < MAX_DATA_PAGES; i++)
		p->data_page[i] = NULL;

	p->n_data_page = 0;
}

int get_name_process(char * buffer, process * p) {
	return strcpy(buffer, p->name);
}

process * create_process(uint64_t new_process_rip, uint64_t params, const char * name) {

	process * new_process = (process *) get_page(sizeof(* new_process));

	strcpy(new_process->name, name);

	new_process->stack_page = get_stack_page(); /* Pide al MemoryAllocator espacio para el stack */

	new_process->entry_point = new_process_rip;

	new_process->st = READY;

	new_process->rsp = fill_stack(new_process_rip, new_process->stack_page, params);

	clean_data_page(new_process);

	insert_process(new_process); /* Agerga proceso a la tabla de procesos. Adentro usa un lock. */

	if (new_process->pid != 0) /* No es el primer proceso */
		new_process->ppid = pid_process(get_current_process());
	else
		foreground = new_process; /* Pone en foreground al primer proceso */

	new_process->open_fds = 0;
	
	printString("Created new process: ");
	printString("\n");
	process_print(new_process);
	return new_process;
}

void add_data_page(process * p, void * page) {
	int i = 0;

	while (i < MAX_DATA_PAGES && p->data_page[i] != NULL)
		i++;

	if (i < MAX_DATA_PAGES) {
		p->n_data_page += 1;
		p->data_page[i] = page;
	}
}

void remove_data_page(process * p, void * page) {
	int i = 0;

	while (i < MAX_DATA_PAGES && p->data_page[i] != page) {
		i++;
	}

	if (i < MAX_DATA_PAGES) {
		p->n_data_page -= 1;
		p->data_page[i] = NULL;
	}
}

void * stack_page_process(process * p) {
	return (void *) p->stack_page;
}

/* Copia en page_array las paginas de forma contigua y termina con NULL */
void data_pages_process(process * p, void * page_array[]) {
	int i, j;
	int pages_left = p->n_data_page;


	for (i = j = 0; i < MAX_DATA_PAGES && pages_left > 0; i++) {
		if (p->data_page[i] != NULL) {
			pages_left -= 1;
			page_array[j++] = p->data_page[i];
		}
	}

	page_array[j] = NULL;
}

process * get_process_by_pid (uint64_t pid) {
	if (pid < MAX_PROCESSES && process_table[pid] != NULL && !is_delete_process(process_table[pid]))
		return process_table[pid];

	return NULL;
}

void destroy_process(process * p) {
	if (p != NULL) {
		lock_table();
		n_processes--;
		free_data_pages(p);
		if(foreground == p)
			set_foreground_process(process_table[p->ppid]);
		process_table[p->pid] = NULL;
		unlock_table();
		store_stack_page(p->stack_page);
		store_page((uint64_t) p);
	}
}

/* Libera las páginas de datos usadas por el proceso. */
static void free_data_pages(process * p) {
	int i;

	for(i = 0; i < MAX_DATA_PAGES && p->n_data_page > 0; i++) {
		if (p->data_page[i] != NULL) {
			store_page((uint64_t) p->data_page[i]);
			p->n_data_page -= 1;
		}
	}
}

/* Marca el proceso para borrar. El scheduler se encarga de quitarlo de su estructura. */
int kill_process(process * p) {
	if (p != NULL && p->pid != 1 && p->pid != 0)
		p->st = DELETE;

	return p != NULL;
}

int is_delete_process(process * p) {
	if (p != NULL)
		return p->st == DELETE;
	return 0;
}

void set_rsp_process(process * p, uint64_t rsp) {
	if (p != NULL)
		p->rsp = rsp;
}

uint64_t get_rsp_process(process * p) {
	if (p != NULL)
		return p->rsp;
	return -1;
}

uint64_t pid_process(process * p) {
	if (p != NULL)
		return p->pid;
	return -1;
}

uint64_t ppid_process(process * p) {
	if (p != NULL)
		return p->ppid;
	return -1;
}

void block_process(process * p) {
	if (p != NULL && p->st != DELETE)
		p->st = BLOCKED;
}

void unblock_process(process * p) {
	if (p != NULL && p->st != DELETE)
		p->st = READY;
}

int is_blocked_process(process * p) {
	if (p != NULL)
		return p->st == BLOCKED || p->st == BLOCKED_READ || p->st == BLOCKED_FOREGROUND;
	return 1;
}

void unblock_read_process(process * p) {
	if (p->st == BLOCKED_READ)
		unblock_process(p);
}

void block_read_process(process * p) {
	p->st = BLOCKED_READ;
}

void set_foreground_process (process * p) {
	if (foreground == get_current_process())
		set_foreground_force_process(p);
}

void set_foreground_force_process (process * p) {
	if (p != NULL && p->pid != 0) {
		foreground = p;
		unblock_foreground_process(p);		
	}
}

static void unblock_foreground_process(process * p) {
	if (p != NULL && p->st == BLOCKED_FOREGROUND)
		p->st = READY;
}

void block_foreground_process(process * p) {
	if (p != NULL)
		p->st = BLOCKED_FOREGROUND;
}

process * get_foreground_process() {
	return foreground;
}

int set_file_open(process * p, int fd) {
	if (fd >= MAX_FDS)
		return 0;
	p->open_fds |= 1 << fd; // Settea bit en posicion fd en 1
	return 1;
}

int set_file_closed(process * p, int fd) {
	if (fd >= MAX_FDS)
		return 0;
	p->open_fds &= ~(1 << fd); // Settea bit en posicion fd en 0
	return 1;
}

int file_is_open(process * p, int fd) {
	return fd < MAX_FDS && CHECK_BIT(p->open_fds, fd);
}

uint64_t number_processes() {
	return n_processes;
}

/* Llena el stack para que sea hookeado al cargar un nuevo proceso
** https://bitbucket.org/RowDaBoat/wyrm */
static uint64_t fill_stack(uint64_t rip, uint64_t stack_page, uint64_t params) {
	StackFrame * frame = (StackFrame *) stack_page - 1;

	frame->gs =		0x001;
	frame->fs =		0x002;
	frame->r15 =	0x003;
	frame->r14 =	0x004;
	frame->r13 =	0x005;
	frame->r12 =	0x006;
	frame->r11 =	0x007;
	frame->r10 =	0x008;
	frame->r9 =		0x009;
	frame->r8 =		0x00A;
	frame->rsi =	0x00B;
	frame->rdi =	params;
	frame->rbp =	0x00D;
	frame->rdx =	0x00E;
	frame->rcx =	0x00F;
	frame->rbx =	0x010;
	frame->rax =	0x011;
	frame->rip =	rip;
	frame->cs =		0x008;
	frame->eflags = 0x202;
	frame->rsp =	(uint64_t)&(frame->base);
	frame->ss = 	0x000;
	frame->base =	0x000;

	return (uint64_t) &frame->gs;
}

/*Debugging*/

void process_print(process * p){
	printString("PID=");
	ncPrintDec(p->pid);
	printString(" Process name= ");
	printString(p->name);
	printString("\n");
	printString("Stack page= ");
	ncPrintDec(p->stack_page);
	printString("\n");
	printString("RIP= ");
	ncPrintDec(p->entry_point);
	printString("\n");
	printString("RSP= ");
	ncPrintDec(p->rsp);
	printString("\n");
}