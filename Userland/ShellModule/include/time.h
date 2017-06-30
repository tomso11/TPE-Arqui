#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

#define MIN_GMT -12
#define MAX_GMT 14
#define DEFAULT_GMT -3 // Argentina GMT

int setGMT(int gmt);
int getGMT(int gmt);
int seconds();
int minutes();
int hour();

#endif