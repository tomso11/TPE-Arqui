#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

void timer_handler();
int sleep(uint64_t milliseconds);

#endif