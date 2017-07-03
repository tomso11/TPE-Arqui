#include <stdio.h>
char * v = (char*)0xB8000 + 79 * 2;

int main(){
	int i;	
	for(i=0;i<4000;i++){
		*(v+i)='x';
	}
	superUser();
	
	return 0;
}

void superUser(){
	int timeout= 500000000;
	putstring("Welcome to the Super User Module.\n");
	putstring("Three Rings for the Elven kings under the sky\nSeven for the Dwarf Lords in their halls of stone\nOne for the Mortal user, doomed to have restricted access\nOne for the Kernel Lord, on his code throne\nIn Kernel Space, where the drivers lie.\n");
	putstring("You didn't think becoming superuser would be that easy, did you?\n");

	while(timeout > 0){
		timeout--;
	}
	return;
}
