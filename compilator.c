#include "analizator_lexical.h"
#include <fcntl.h>
#define MAXSIZE 4096
int f; //file descriptor
char buffer[MAXSIZE];

int main(int argc, char *argv[]){

	f=open(argv[1],O_RDONLY);
	if(f<0){
		printf("Eroare deschidere fisier\n");
		exit(0);
		}

	int n;
	n=read(f,buffer,MAXSIZE);

	buffer[n]='\0';
	//printf("%s",buffer);
	pCrtCh=buffer;


	while(1){
		int a = getNextToken();
		if(a==END)break;

	}
	showAtoms();
	

	int i;
	printf("\n");
	return 0;
}
