#include <stdio.h>
#include <editor.h>

int main(){
	clearEditor();

	char ** s = malloc(sizeof(char **));
	char * editor = "chomp\n";
	s[0]= editor;
	printFF("%s",s, NULL);
	char c;
	int exitFlag = 0;
	while(!exitFlag){
		c = getchar();
		if(c == '%'){
			char ** ss = malloc(sizeof(char **));
			char * sos = malloc(30);
			ss[0] = sos;
			scanFF("%s",ss, NULL);
			if(strcmp(sos,"exit") == 0){
				exitFlag = 1;
				clearEditor();
			}
		}
	}

	return 0;
}



void clearEditor(){
	sys_call(5,0,0);
}








