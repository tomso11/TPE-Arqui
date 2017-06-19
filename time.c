#include <time.h>

//no se usa, pero si quieren podemos hacer que pueda cambiar el gmt desde la shell
int setGMT(int gmt) {
	if (gmt >= MIN_GMT && gmt <= MAX_GMT) {
		DEFAULT_GMT = gmt;
		return 1;
	}
	return 0;
}

//no se usa, pero si quieren podemos hacer que pueda pedir el gmt desde la shell
int getGMT(int gmt) {
	return DEFAULT_GMT;
}


int seconds() {
    return time(0);
}


int minutes() {
    return time(1);
}


int hour() {
   	int h = time(2) + DEFAULT_GMT;
	if (h >= 0 && h <= 23)
		return h;
	return h < 0 ? h + 24 : h - 24;
}