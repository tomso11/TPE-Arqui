#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);

void * malloc (int size);
void free(void * ptr);
int atoi(const char *str);
int itoa(int value, char *str, int base);

#endif