#ifndef DIRS_H
#define DIRS_H

#include "memoryAllocator.h"

/* Direccion de inicio del módulo de código */
#define CODE_ADDRESS 0x400000

/*Direccion de inicio del módulo de datos */
#define DATA_ADDRESS 0x500000

/* Comienzo del stack del user */
#define STACK_ADDRESS (MEMORY_PAGES_END + PAGE_SIZE)


#endif