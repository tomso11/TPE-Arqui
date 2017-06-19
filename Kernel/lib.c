#include <stdint.h>
#include <ctype.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

void * malloc (int size) {
	return (void *) res(size); // res es un syscall 
}


void free(void * ptr){
	return;
}


/* A partir de un string retorna su valor entero */
int atoi(const char *str) {
	while (isspace(*str))
		str++;

	int num = 0;
	int sign = 1;
	if (*str == '-') {
		str++;
		sign = -1;
	}
	else if (*str == '+')
		str++;

	while (*str != '\0' && isdigit(*str)) {
		num = num * 10 + (*str) - '0';
		str++;
	}

	return num * sign;
}

/* Copia en str los valores ascii de los digitos de value en la base indicada.
** Devuelve la cantidad de digitos copiados. */
int itoa(int value, char *str, int base) {
	char *s = str;
	char *s1, *s2;
	int length = 0;

	if(value < 0 && base == 10) {
		value = -value;
		length++;
		*s++ = '-';
		str++;
	}

	//Calculate characters for each digit
	do
	{
		int rest = value % base;
		*s++ = (rest < 10) ? rest + '0' : rest + 'A' - 10;
		length++;
	}
	while (value /= base);

	// Terminate string in str.
	*s = '\0';

	//Reverse string in str.
	s1 = str;
	s2 = s - 1;
	while (s1 < s2)
	{
		char tmp = *s1;
		*s1 = *s2;
		*s2 = tmp;
		s1++;
		s2--;
	}

	return length;
}
