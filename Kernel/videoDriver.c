static int res_x=0;
static int res_y=0;
static int cursor=0;


#define ROW(p) ((p)/WIDTH)
#define COL(p) ((p)%WIDTH)
#define CURSOR_LIMIT HEIGHT*WIDTH


/* Equivale a mover cada linea una fila hacia arriba. La línea de la primer fila es perdida. */
static void scroll() {
	int line=HEIGHT-1;
	char lineText[WIDTH]= //we aca hay que elegir la primera linea
	char aux[WIDTH]=null;
	while(line != 0 ){
		memcpy(lineText, aux, WIDTH);
		clear_line(line);
		line--;
		lineText= // copia el contenido de la linea anterior
		//copio aux en linea
	}
}

/* Limpia una linea, es decir, coloca espacios en toda la fila */
static void clear_line(int line) {
	if (line >= 0 && line < HEIGHT) {
		int i;
		for (i=0; i < WIDTH; i++)
			print_char(' ', line, i);
	}
}

/* Imprime un caracter*/
void put_char(char c) {
	if (c == '\b') {
		cursor--;
		print_char(' ', ROW(cursor), COL(cursor));
	}
	else if (c == '\n') {
		cursor += WIDTH - COL(cursor); /* cursor al comienzo de nueva linea */
	}
	else if (c == '\r') { /* reset cursor */
		cursor = 0;
	} 
	else if (c != '\t') { /* tab no hace nada */
		print_char(c, ROW(cursor), COL(cursor));
		cursor++;
	}
	
	if (cursor < 0)
		cursor = 0;
	if (cursor >= CURSOR_LIMIT) {
		scroll();
		cursor = CURSOR_LIMIT - WIDTH;
	}
}