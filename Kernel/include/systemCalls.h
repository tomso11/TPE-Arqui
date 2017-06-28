#ifndef _SYSTEMCALLS_H_
#define _SYSTEMCALLS_H_

#include <stdint.h>
#include "driverVideo.h"

#define STDOUT 0
#define STDIN 1
#define STDERR 2
#define SYSCALL_S (sizeof(syscalls)/sizeof(syscalls[0]))
/*Direccion de inicio del módulo de datos */
#define DATA_DIR 0x500000



static uint64_t sys_write_wr(uint64_t fds, uint64_t str, uint64_t len);
static uint64_t sys_read_wr(uint64_t fds, uint64_t buffer, uint64_t len);
static uint64_t sys_malloc_wr(uint64_t bytes, uint64_t unused1, uint64_t unused2);
static uint64_t sys_data_address_wr(uint64_t unused1, uint64_t unused2, uint64_t unused3);
static uint64_t sys_time_wr(uint64_t selection, uint64_t unused1, uint64_t unused2);
static uint64_t sys_clear_wr(uint64_t unused1, uint64_t unused2, uint64_t unused3);

uint64_t syscallDispatcher(uint64_t rax, uint64_t rbx, uint64_t rdx, uint64_t rcx);
uint64_t sys_read(uint64_t fds, char * buffer, uint64_t bytes);
uint64_t sys_write(uint64_t fds, const char  str, uint64_t length);
uint64_t sys_malloc(uint64_t bytes);
uint64_t sys_data_dir();
uint64_t sys_time(uint64_t selection);
uint64_t sys_clear();

#endif
