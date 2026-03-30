#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "supporto.h"




int variabili_controllate = 0;
int errori_rilevati = 0;
int variabili_inutilizzate = 0;
int nomi_variabili_non_corretti = 0;
int tipi_dato_scorretti = 0;

const char* tipi_base[] = {"int", "float", "double", "char", "long", "bool", NULL};

bool isMain(char* parola){ // controlla se è funzione main
	int numeroParola=strlen(parola);
	if(strncmp(parola,"main(",5)==0 && parola[numeroParola-1]=='{'){
		return true;
	}
	return false;
}

bool controlloTipo(char* parola){
	if(strcmp(parola,"char")==0){
		return true;
	}else
        if(strcmp(parola,"int")==0){
                return true;
        }else
        if(strcmp(parola,"float")==0){
                return true;
        }else
        if(strcmp(parola,"boole")==0){
                return true;
        }

	tipi_dato_scorretti++;

        return false;
}


//controlla il tipo delle variabili
char*  controlloVariabile(char* filename, bool opzione_output, bool opzione_verbose){
	FILE *file=fopen(filename,"r"); //puntatore al contenuto del file

	//controllo se file valido
	if(file == NULL){
		printf("[ControlloVariabile] errore apertura file\n");
		return NULL;
	}


	char riga[128];	//numero massimo di parole in una riga, buffer per una riga
//	int numero_parole_riga_corrente;	//numero parole della riga corrente
//	int numeroRiga=1;	//numero riga attuale

	int numero_pv;//punto virgola
	int numero_s;//spazzio
	int numeroRiga=0;

	bool esisteMain=false;
	//legge file riga per riga
	while(fgets(riga, sizeof(riga), file) != NULL){	//affinche una riga non è vuota nota: fgets() include il \n della riga

		riga[strcspn(riga, "\n")] = '\0';	//rimuove \n della riga e rimpiazza con \0

		//split della riga in parole

		int contaSpazzi=0;
                while(riga[contaSpazzi]==' '||riga[contaSpazzi]=='\t'){
                        contaSpazzi++;
                }
                char rigaPul[strlen(riga)-contaSpazzi+1];// riga pulita lunghezza della riga - numero di spazzi + 1 posto per \0
                strncpy(rigaPul, riga + contaSpazzi, strlen(riga) - contaSpazzi + 1);

//_____divisione per punto virgola_____ ("int i;char c;")==>[[int i],[char c]]
                char **parole_split_pv = split(rigaPul,";",&numero_pv);
		//salta riga vuota
		if(numero_pv==0){
                //      printf("riga vuota\n");
                        free(parole_split_pv);
                        numeroRiga++;
                        continue;
                }
		


//manca controllo di # e //






	//_____divisione per spazi_____[[int i],[char c]]==>[[[int],[i]],[[char],[c]]]

		for(int i=0;i<numero_pv;i++){
                //      printf("%s\n",parole_split_pv[i]);

                        char *cp=parole_split_pv[i];//puntatore cp punta parole corrente se i=0 cp-->[int i]
                        char **parole_split_s=split(cp," ",&numero_s);
			bool tipoCorretto;
	                for(int j=0;j<numero_s;j++){
				if(j==0){
					tipoCorretto=controlloTipo(parole_split_s[j]);
                                        if(!tipoCorretto){
   	                                     printf("[TYPEERRORE] riga:%i\n",numeroRiga);
					}
                                }
                                if(j==1){
                                        if(tipoCorretto){
                                                if(isMain(parole_split_s[j])){
                                                      //  printf("è main");
                                                        if(!esisteMain){
                                                                esisteMain=true;
                                                                //printf("è main");
                                                        }
                                                        else{
								errori_rilevati++;
                                                                printf("errore doppia main\n");
                                                        }
                                                }
                                                else
						{
                                                //      printf("è variabile");
                                                        bool es1=false; // se esiste "(" es1 es2 per controlare una funzione
                                                        bool es2=false; // se esiste ")"

                                                        for(int numpar=0;numpar<strlen(parole_split_s[j]);numpar++){

                                                                if(numpar==0){
                                                                        if(!(((int)parole_split_s[j][numpar]>=65 && (int)parole_split_s[j][numpar]<=90) || ((int)parole_split_s[j][numpar]>=97 && (int)parole_split_s[j][numpar]<=122) || parole_split_s[j][numpar]=='_')){
										nomi_variabili_non_corretti++;
                                                                              //  printf("nome errore");
                                                                              //  printf("%c%i\n",parole_split_s[j][numpar],(int)parole_split_s[j][numpar]);
                                                                                break;
                                                                        }
                                                                }else
                                                                if(parole_split_s[j][numpar]=='='){
                                                                        break;
                                                                }else
                                                                if((int)parole_split_s[j][numpar]=='('){
                                                                        es1=true;
                                                                }else
                                                                if((int)parole_split_s[j][numpar]==')' && es1){
                                                                        es2=true;
                                                                }else
                                                                if(!(((int)parole_split_s[j][numpar]>=65 && (int)parole_split_s[j][numpar]<=90) ||((int)parole_split_s[j][numpar]>=97 && (int)parole_split_s[j][numpar]<=122) || parole_split_s[j][numpar]=='_') || ((int)parole_split_s[j][numpar]>=48 && (int)parole_split_s[j][numpar]<=57)){
                                                                        printf("nome errore");
                                                                        printf("%c%i\n",parole_split_s[j][numpar],(int)parole_split_s[j][numpar]);
                                                                        break;
                                                                }
                                                                if(es1 && es2){
                                                                        printf("funzione");
									break;
                                                                }

                                                        }
                                                }
                                        }else
					{
						printf("[tipo er]");
					}
                                }
                        }
                        free(parole_split_s);
		}
	numeroRiga++;
	free(parole_split_pv);
	}
	printf("[ControlloVariabile] termine controllo variabili\n");
	return NULL;
}


//controlla variabili inutilizzate
List* controllaVarInutilizzate(char *nome_file_in, Statistiche *stats){
	List* variabili = list_create();	//lista per tenere traccia le variabili

	FILE* file = fopen(nome_file_in, "r");
	if (file == NULL) {
		printf("[ControlloVariabile] controlla_var_inutilizzate: errore di apertura file\n");
		return NULL;
	}

	char riga[512]; //buffer per leggere una riga alla volta
	int riga_attuale = 0;

	//ciclo principale che scorre le righe del file
	//nota: fgets(char *str, int n, FILE *stream):
	//	str: buffer dove viene salvata la stringa letta
	//	n: numero massimo di caratteri da leggere
	//	stream: sorgente da cui leggere
	//
	//si ferma sempre al \n, che viene incluso.
	//aggiunge il terminatore \0 alla fine della riga
	//restituisce str in caso di successo, NULL in caso di errore o fine file.
	while (fgets(riga, sizeof(riga), file) != NULL) {	
		riga_attuale ++;
		//pulizia riga
		riga[strcspn(riga, "\n")] = '\0';	//rimuove \n
		char *riga_pulita = rimuoviSpaziSx(riga);

		//se la riga corrente è commento o #include o vuota, salta.
		if(controllaRigaCommento(riga_pulita) == true){
			printf("[controllaVarInutilizzata]: commento  alla riga %i\n", riga_attuale);
			continue;
		}
		else if(controllaRigaInclude(riga_pulita) == true){
			printf("[controllaVarInutilizzata]: include  alla riga %i\n", riga_attuale);
			continue;
		}
		else if(controllaRigaVuota(riga_pulita) == true){
			printf("[controllaVarInutilizzata]: riga %i vuota\n", riga_attuale);
			continue;
		}

		//ottieni i token per la riga
		int numero_token;
		char** tokens = split(riga_pulita, " \t;", &numero_token);

		/*
		for(int i = 0; i< numero_token; i++){
			printf("[TOKENS] : %s \n", tokens[i]);
		}
		*/
		




		//controlla se è una dichiarazione valida di variabile, e lo mette nella lista variabili
		for (int i = 0; i < numero_token; i++){	//per ogni token
			//controlla se tokens[i] è un tipo
			for (int t = 0; tipi_base[t]; t++){	//per ogni tipo
				if (strcmp(tokens[i], tipi_base[t]) == 0 && i+1 < numero_token){
					//tronca il nome dell variabile al primo carattere non alfanum (es. =, ;, [)
					char *candidato = pulisciNomeVariabile(tokens[i+1]);

					//controlla se il token corrente è una funzione
					//printf("[DEBUG] CANDIDATO var CORRENTE : %s\n", candidato);
					if (strchr(tokens[i+1], '(') != NULL){
    					free(candidato);
						printf("funzione rilevata alla riga %i\n", riga_attuale);
   					 	break;  // è una funzione, salta
					}


					//filtra puntatori, array, main
					if(candidato[0] != '*' && candidato[0] != '[' && strcmp(candidato, "main") != 0 && isalpha(candidato[0])){
						Variabile* var = malloc(sizeof(Variabile));
						var->nome = strdup(candidato);
						var->riga_dichiarata = riga_attuale;
						var->usata = false;

						list_append(variabili, var);
						printf("[ControllaVarInutilizzate] dichiarata '%s' alla riga %d\n", var->nome, var->riga_dichiarata);
					}
					break; //una dichiarazione alla volta
				}
			}
		}
	} 


	//secondo passaggio 
	rewind(file);
	riga_attuale = 0;

	while (fgets(riga, sizeof(riga), file)) {
		riga_attuale++;
		riga[strcspn(riga, "\n")] = '\0';
		char *riga_pulita = rimuoviSpaziSx(riga);
		//se la riga corrente è commento o #include o vuota, salta.
		if (controllaRigaCommento(riga_pulita) == true || controllaRigaInclude(riga_pulita) == true || controllaRigaVuota(riga_pulita) == true){
			continue;
		}

		int numero_token;
		char **tokens = split(riga_pulita, " \t=;,()[]{}+-*/<>!&|", &numero_token);

			//per ogni riga scorre una volta le variabili della lista
		for (int i = 0; i < variabili->numero_elementi_attuali; i++)
		{
			Variabile *var_att = list_get(variabili, i);
			if (var_att->usata == true){
				continue;
			}
			//scorre i token della riga attuale
			for (int t = 0; t < numero_token; t++){
				char *token_pulito = pulisciNomeVariabile(tokens[t]);
				if (strcmp(token_pulito, var_att->nome) == 0 && riga_attuale != var_att->riga_dichiarata){
					//se il token è uguale al nome della variabile e la riga attuale non è la riga in cui la var è stata dichiarata
					var_att->usata = true;	//allora la var è stata utilizzata
					printf("variabile %s utilizzata alla riga %i\n", var_att->nome, riga_attuale);
				}
				free(token_pulito);
			}
		}
		free(tokens);
	}

		//controllo finale delle variabili non utilizzate
		for (int i = 0; i < variabili->numero_elementi_attuali; i++){
			Variabile *var = list_get(variabili, i);
			if (var->usata == false){
				printf("[ControllaVarInutilizzate] variabile %s non usata\n", var->nome);
				stats->variabili_inutilizzate++;
			}
		}
	fclose(file);
	return variabili;

}








