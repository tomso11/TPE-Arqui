#include "memoryAllocator.h"
#include "processManager.h"
#include "process.h"
#include "kernelMutex.h"

/* Alocador de paginas para datos */

static page_index_t pages_stack[ STACK_SIZE ];
static page_index_t current_index = 0; /* Apunta al elemento a poppear */

static uint64_t memory_mutex_key = -1;
static uint64_t stack_memory_mutex_key = -1;


/* Se inicializan los indices en el stack */
/* Comienza guardando en 1 porque al comienzo de data están los parametros de los fractales */
void initialize_memory_allocator() {
	int i;
	for (i = 0; i < STACK_SIZE-1; i++)
		pages_stack[i] = i+1;
}

void initialize_memory_allocator_mutex() {
	memory_mutex_key = mutex_open("__MEMORY_MUTEX__");	
}

/* Se ignora el valor de size. Se asume que será menor a PAGE_SIZE.
** Se hizo así para que la interfaz tenga sentido, en un futuro puede
** cambiar la implementación a una que si contemple el size correctamente.
*/
uint64_t get_page(uint64_t size) {
	mutex_lock(memory_mutex_key);
	if (current_index < STACK_SIZE) {
		uint64_t page = pages_stack[current_index++] * PAGE_SIZE + DATA_ADDRESS;
		mutex_unlock(memory_mutex_key);
		return page;
	}
	mutex_lock(memory_mutex_key);
	return 0;
}

uint64_t store_page(uint64_t address) {
	mutex_lock(memory_mutex_key);
	if (current_index > 0) {
		pages_stack[--current_index] = (address - DATA_ADDRESS) / PAGE_SIZE;
		mutex_unlock(memory_mutex_key);
		return 1;
	}
	mutex_unlock(memory_mutex_key);
	return 0;
}

static uint64_t stack_pages_stack[ MAX_PROCESSES ];
static uint16_t current_stack_index = 0; /* Apunta al elemento a poppear */

/* Alocador de paginas para stack de procesos */

void initialize_stack_memory_allocator() {
	int i;
	for (i = 0; i < MAX_PROCESSES; i++)
		stack_pages_stack[i] = (i+1) * STACK_PAGE_SIZE + STACK_ADDRESS; /* Se suma 1 porque el stack crece 
																		** hacia direcciones menores */
}

void initialize_stack_memory_allocator_mutex() {
	stack_memory_mutex_key = mutex_open("__STACK_MEMORY_MUTEX__");
}

uint64_t get_stack_page() {
	mutex_lock(stack_memory_mutex_key);
	if (current_stack_index < MAX_PROCESSES) {
		uint64_t page = stack_pages_stack[current_stack_index++];
		mutex_unlock(stack_memory_mutex_key);
		return page;
	}
	mutex_unlock(stack_memory_mutex_key);
	return 0;
}

uint64_t store_stack_page(uint64_t address) {
	mutex_lock(stack_memory_mutex_key);
	if (current_stack_index > 0) {
		stack_pages_stack[--current_stack_index] = address;
		mutex_unlock(stack_memory_mutex_key);
		return 1;
	}
	mutex_unlock(stack_memory_mutex_key);
	return 0;
}