static int res_x=0;
static int res_y=0;
static int cursor=0;


#define ROW(p) ((p)/WIDTH)
#define COL(p) ((p)%WIDTH)
#define CURSOR_LIMIT HEIGHT*WIDTH

// esto serviria para el bitmap
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

//habria que ver si esto hacerlo con el cursor o con posicion designada
void put_char(char c, int row, int col){
	if( valid_pos(row, col) ){
		print_char(c,row,col); // en asm
	}
	cursor++;
}

