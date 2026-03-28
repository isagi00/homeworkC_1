#include <stdio.h>
#include <string.h>
#include "supporto.h"
#include <stdlib.h>

char*  controlloVariabile(char* filename){
	FILE *file=fopen(filename,"r");
	int numero;
	int numeroRiga=0;
	if(file == NULL){
		return "ERRORE apertura file\n";
	}
	char riga[100];
	while(fgets(riga,sizeof(riga),file)!=NULL){
		char **parola=split(riga,&numero);
		for(int i=0;i<numero;i++){
				if(strcmp(parola[i],"")==0){
					i++;
				
				}
			for(int j=0;j<strlen(parola[i]);j++){
				if(parola[i][j]=='#'|| (parola[i][j]=='/' && parola[i][j+1]=='/')){
					printf("è include o com\n");
					i=i+1;
				
				}
			}
if(i==0){
                                        if((strcmp(parola[i],"int")*strcmp(parola[i],"long")*strcmp(parola[i],"short"))==0){
                                                printf("tipo intero");
                                         }
                                         else{
                                                 if(strcmp(parola[i],"char")==0){
                                                         printf("tipo char");
                                                 }
                                                 else{
                                                         if(strcmp(parola[i],"float")==0){

                                                         }
                                                         else{
                                                                 if(strcmp(parola[i],"bool")==0){
                                                                 }
                                                                 else
                                                                 {
                                                                          printf("[ERROR TYPE] riga %i\n",numeroRiga);
                                                                 }
                                                         }
                                                 }
                                         }
                                  }

			numeroRiga++;
		}
		free(parola);
	}
	printf("fine\n");
	return NULL;
}

int main(){
	controlloVariabile("prov.c");
return 0;
}
