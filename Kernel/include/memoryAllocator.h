#ifndef _MEM_ALLOCATOR_
#define _MEM_ALLOCATOR_

#include <stdint.h>
#include "dirs.h"
#include "defs.h"


/* Paginas de 4KB */
#define PAGE_SIZE 0x1000

/* Tamaño de stack de nuevos procesos */
#define STACK_PAGE_SIZE 0x100000

typedef uint8_t page_index_t;

#define STACK_SIZE ((page_index_t) - 1) /* valor máximo que puede tomar una variable de tipo page_index_t */
#define MEMORY_PAGES_END ((STACK_SIZE + 1)* PAGE_SIZE + DATA_ADDRESS)

void initialize_memory_allocator();
void initialize_memory_allocator_mutex();
uint64_t get_page(uint64_t size);
uint64_t store_page(uint64_t address);

void initialize_stack_memory_allocator();
void initialize_stack_memory_allocator_mutex();
uint64_t get_stack_page();
uint64_t store_stack_page(uint64_t address);




#endif
