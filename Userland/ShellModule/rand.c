#include "rand.h"

static unsigned long int next = 1;

int rand_int() {
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

int rand_int_range(int low, int top) {
  if (top <= low)
    return -1;
  int r = top - low;
  return (rand_int() % r) + low;
}

void srand(unsigned int seed) {
  next = seed;
}
