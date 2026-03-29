#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sup.h"

int variabili_controllate = 0;
int errori_rilevati = 0;
int variabili_inutilizzate = 0;
int nomi_variabili_non_corretti = 0;
int tipi_dato_scorretti = 0;

bool isMain(char* parola){
	printf("sono qui");
	int i=strlen(parola);
	if(strncmp(parola,"main(",5)==0 && parola[i-1]=='{'){
		return true;
	}
	return false;
}



char* controlloVariabile(char* tipo){
        if(strcmp(tipo,"char")==0){
                return "tipo char";
        }else
        if(strcmp(tipo,"int")==0){
                return "tipo int";
        }else
        if(strcmp(tipo,"float")==0){
                return "tipo float";
        }else
        if(strcmp(tipo,"boole")==0){
                return "tipo boole";
        }

        return "ERRORE TYPE";
}
/*
char* eliminaSpazziD(char* s){

	int i=0;
	while(s[i]==' '||s[i]=='\t'){
		i++;
	}
	char *nuovaS=malloc(strlen(s)-i+1);

	strncpy(nuovaS,s+i,strlen(s)-i+1);
	return nuovaS;
}
*/
char* cont(char* filename){

	FILE *file=fopen(filename,"r");

	if(file == NULL){
		printf("[ControlloVariabile] errore");
		return NULL;
	}

	char riga[128];
	int numero_pv;
	int numero_s;
	int numeroRiga=0;

	bool esisteMain=false;

	while(fgets(riga, sizeof(riga), file)!=NULL){

		riga[strcspn(riga, "\n")]='\0';

		int contaSpa=0;
                while(riga[contaSpa]==' '||riga[contaSpa]=='\t'){
                        contaSpa++;
                }
                char rigaPul[strlen(riga)-contaSpa+1];
                strncpy(rigaPul, riga + contaSpa, strlen(riga) - contaSpa + 1);
		char **parole_split_pv = split(rigaPul,";",&numero_pv);
		if(numero_pv==0){
		//	printf("riga vuota\n");
			free(parole_split_pv);
			numeroRiga++;
			continue;
		}


		for(int i=0;i<numero_pv;i++){
		//	printf("%s\n",parole_split_pv[i]);

			char *cp=parole_split_pv[i];
			char **parole_split_s=split(cp," ",&numero_s);
			char *k;
			for(int j=0;j<numero_s;j++){
			//	printf("parola:%s numero%i\n",parole_split_s[j],j);
// conteollo tipo variabile:
				if(j==0){
					k=controlloVariabile(parole_split_s[j]);
					printf("%s riga:%i\n",k,numeroRiga);
				}
				if(j==1){
					if(strcmp(k,"ERRORE TYPE")!=0){
						if(isMain(parole_split_s[j])){
							printf("è main");
							if(!esisteMain){
								esisteMain=true;
								//printf("è main");
							}
							else{
								printf("errore doppia main");
							}
						}
						else{
						//	printf("è variabile");
	                                                bool es1=false;
                                                        bool es2=false;

							for(int numpar=0;numpar<strlen(parole_split_s[j]);numpar++){

								if(numpar==0){
									if(!(((int)parole_split_s[j][numpar]>=65 && (int)parole_split_s[j][numpar]<=90) || ((int)parole_split_s[j][numpar]>=97 && (int)parole_split_s[j][numpar]<=122) || parole_split_s[j][numpar]=='_')){
										printf("nome errore");
										printf("%c%i\n",parole_split_s[j][numpar],(int)parole_split_s[j][numpar]);
										break;
									}
								}else
								if(parole_split_s[j][numpar]=='='){
									break;
								}else
								if((int)parole_split_s[j][numpar]=='('){
									es1=true;
                                                                }else
								if((int)parole_split_s[j][numpar]==')'){
									es2=true;
								}else
								if(!(((int)parole_split_s[j][numpar]>=65 && (int)parole_split_s[j][numpar]<=90) ||((int)parole_split_s[j][numpar]>=97 && (int)parole_split_s[j][numpar]<=122) || parole_split_s[j][numpar]=='_') || ((int)parole_split_s[j][numpar]>=48 && (int)parole_split_s[j][numpar]<=57)){
									printf("nome errore");
                                                                        printf("%c%i\n",parole_split_s[j][numpar],(int)parole_split_s[j][numpar]);
                                                                    	break;
								}
								if(es1 && es2){
									printf("funzione");
								}

							}
						}
					}
				}
			}
			free(parole_split_s);
		}
	numeroRiga++;
	free(parole_split_pv);
	}
	return NULL;
}


int main (){
	cont("prov.c");
	return 0;
}
