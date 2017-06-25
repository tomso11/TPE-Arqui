#ifndef _STRINGLIB_H_
#define _STRINGLIB_H_

#include <limits.h>

int comp_str(const char* s, const char *t);
int strncmp(const char *s, const char *t, unsigned int num);
int strcmp(const char * s, const char * t);
int strcpy(char * to, const char * from);
int strcpyto(char * to, const char * from, char limit);
int strcpynto(char * to, const char * from, char limit, unsigned int n);
int str_len(const char *str);

#endif
