#include <stdio.h>
#include <string.h>
#include "supporto.h"
#include <stdlib.h>
#include <stdbool.h>

char*  controlloVariabile(char* filename, bool opzione_output, bool opzione_verbose){
	FILE *file=fopen(filename,"r"); //puntatore al contenuto del file
	int numero;	//numero di righe del file
	int numeroRiga=0;	//numero riga attuale

	//controllo se file valido
	if(file == NULL){
		return "ERRORE apertura file\n";
	}

	char riga[128];	//numero massimo di parole in una riga
	while(fgets(riga,sizeof(riga),file)!=NULL){	//affinche una riga non è vuota
		char **parole = split(riga, &numero);	//separa le parole della riga
		
		//...
		for(int i=0;i<numero;i++){	//per ogni parola
			//controllo riga vuota
			if(strcmp(parole[i],"")==0){	
				i++;
			}

			//controlla se è #include o commento
			for(int j=0;j<strlen(parole[i]);j++){	//per ogni lettera della parola
				if(parole[i][j]=='#'|| (parole[i][j]=='/' && parole[i][j+1]=='/')){
					printf("[ControlloVariabile] è include o com\n");
					i=i+1;
					j = 0;
				}

				//... altri controlli qui
			}
			
			//controllo tipi variabile
			if(i==0){
                if((strcmp(parole[i],"int")*strcmp(parole[i],"long")*strcmp(parole[i],"short"))==0){
                    printf("tipo intero alla riga %i\n", numeroRiga);
                }
                else if(strcmp(parole[i],"char")==0){
					printf("tipo char alla riga %i\n", numeroRiga);
				}
                else if(strcmp(parole[i],"float")==0){
                    printf("tipo float alla riga %i\n", numeroRiga);           
				}
				else if(strcmp(parole[i],"bool")==0){
                    printf("tipo bool alla riga %i\n", numeroRiga);                               
				}
				else{
					printf("[ERROR TYPE] riga %i\n",numeroRiga);
				}
            }

			numeroRiga++; //passa riga successiva

		}
		free(parole);	//libera spazio riservato dell'array parole
	}

	printf("fine\n");
	return NULL;
}

int main(){
	controlloVariabile("prov.c", false, false);
return 0;
}
