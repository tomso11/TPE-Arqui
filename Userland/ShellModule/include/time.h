#ifndef TIME_H
#define TIME_H

#include <stdint.h>

#define MIN_GMT -12
#define MAX_GMT 14
#define DEFAULT_GMT -3 // Argentina GMT

void sleep(uint64_t milliseconds);
int seconds();
int minutes();
int hour();
int setGMT(int gmt);

#endif
