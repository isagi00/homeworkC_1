#include <stdio.h>
#include <string.h>
#include <stdlib.h>
char** sep(char* sp,char* sepa,int *n){
        char **vec = malloc(100*sizeof(char*));
	char *token=strtok(sp,sepa);
	int i=0;
	while(token!=NULL){
		vec[i++]=token;
		token=strtok(NULL,sepa); }
	*n=i;
	return vec;
	}
int main(){
	int numero;
	char riga[]="yuyfy tdhf";
	/asiuas
	char **pp=sep(riga,&numero);
	for(int j=0;j<numero;j++){
		printf("%s\n",pp[j]);
	}
	free(pp);
	return 0;
}










