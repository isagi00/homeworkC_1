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



//controlla il tipo delle variabili
char*  controlloVariabile(char* filename, bool opzione_output, bool opzione_verbose){
	FILE *file=fopen(filename,"r"); //puntatore al contenuto del file

	//controllo se file valido
	if(file == NULL){
		printf("[ControlloVariabile] errore apertura file\n");
		return NULL;
	}


	char riga[128];	//numero massimo di parole in una riga, buffer per una riga
	int numero_parole_riga_corrente;	//numero parole della riga corrente
	int numeroRiga=1;	//numero riga attuale

	//legge file riga per riga
	while(fgets(riga, sizeof(riga), file) != NULL){	//affinche una riga non è vuota nota: fgets() include il \n della riga

		riga[strcspn(riga, "\n")] = '\0';	//rimuove \n della riga e rimpiazza con \0

		//split della riga in parole

		char **parole = split(riga," \t", &numero_parole_riga_corrente);

		//debug
		printf("[DEBUG] riga %d\n", numeroRiga);
		for (int i = 0; i < numero_parole_riga_corrente; i++){
			printf("stringa:[%s] \n", parole[i]);  // stampa ogni stringa tra virgolette
		}

		
		//controllo riga vuota
		if (numero_parole_riga_corrente == 0) {
            printf("[ControlloVariabile] riga %d vuota, skip\n", numeroRiga);
            free(parole);
            numeroRiga++;
            continue;  // salta alla prossima riga
        }

		//controlla se è #include o //commento
		if (parole[0][0] == '#' || (parole[0][0] == '/' && strlen(parole[0]) > 1 && parole[0][1] == '/')) {
            printf("[ControlloVariabile] riga %d: commento/include, skip\n", numeroRiga);
            free(parole);
            numeroRiga++;
            continue;  // salta alla prossima riga
        }
			
		//controllo tipi variabile
		if((strcmp(parole[0],"int") == 0 || strcmp(parole[0],"long") == 0 || strcmp(parole[0], "short") == 0 )){
            printf("[ControlloVariabile] rilevato tipo intero alla riga %i\n", numeroRiga);
			variabili_controllate += 1;
		}
		else if(strcmp(parole[0],"char")==0){
			printf("[ControlloVariabile] rilevato tipo char alla riga %i\n", numeroRiga);
			variabili_controllate += 1;
		}
        else if(strcmp(parole[0],"float")==0){
            printf("[ControlloVariabile] rilevato tipo float alla riga %i\n", numeroRiga); 
			variabili_controllate += 1;          
		}
		else if(strcmp(parole[0],"bool")==0){
            printf("[ControlloVariabile] rilevato tipo bool alla riga %i\n", numeroRiga);         
			variabili_controllate += 1;                      
		}
		else{
			printf("[ControlloVariabile] rilevato errore di tipo alla riga %i\n", numeroRiga);
			errori_rilevati += 1;
		}
		//pulizia fine riga
        numeroRiga++; //passa riga successiva
		free(parole);	//libera spazio riservato dell'array parole   
		}
	
	fclose(file);
	printf("[ControlloVariabile] termine controllo variabili\n");
	return NULL;
}


//controlla variabili inutilizzate
//TODO: la lista variabili viene popolata, ma adesso da controllare l'utilizzo di esse nel codice.
void controllaVarInutilizzate(char *nome_file_in){
	List* variabili = list_create();	//lista per tenere traccia le variabili

	FILE* file = fopen(nome_file_in, "r");
	if (file == NULL) {
		printf("[ControlloVariabile] controlla_var_inutilizzate: errore di apertura file\n");
		return;
	}

	char riga[512]; //buffer per leggere una riga alla volta
	int riga_attuale = 0;

	//ciclo principale che scorre le righe del file
	//nota: fgets(char *str, int n, FILE *stream):
	//	*str: buffer dove viene salvata la stringa letta
	//	n: numero massimo di caratteri da leggere
	//	*stream: sorgente da cui leggere
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
		if (controllaRigaCommento(riga_pulita) == true || controllaRigaInclude(riga_pulita) == true || controllaRigaVuota(riga_pulita) == true){
			printf("[controllaVarInutilizzata]: commento, include o vuoto rilevato alla riga %i\n", riga_attuale);
			continue;
		}

		int numero_token;
		char** tokens = split(riga_pulita, " \t", &numero_token);

		//controlla se è una dichiarazione valida di variabile, e lo mette nella lista variabili
		for (int i = 0; i < numero_token; i++){	//per ogni token
			//controlla se tokens[i] è un tipo
			for (int t = 0; tipi_base[t]; t++){	//per ogni tipo
				if (strcmp(tokens[i], tipi_base[t]) == 0 && i+1 < numero_token){
					//tronca il nome dell variabile al primo carattere non alfanum (es. =, ;, [)
					char *candidato = pulisciNomeVariabile(tokens[i+1]);

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
	fclose(file);

}








//salva statistiche su file esterno
void salva_statistiche_file_esterno(char *nome_file_output){
	FILE *file_pointer = fopen(nome_file_output, "w"); //puntatore a file esterno

	if (file_pointer == NULL) {
		printf("[ControlloVariabile] salva_statistiche_file_esterno : Errore apertura file output\n");
		return;
	}
	//scrittura su file output
	fprintf(file_pointer, "[STAT] numero di variabili controllate: %i\n", variabili_controllate);
	fprintf(file_pointer, "[STAT] numero di errori rilevati: %i\n", errori_rilevati);
	fprintf(file_pointer, "[STAT] numero di nomi delle variabili non corretti: %i\n", nomi_variabili_non_corretti);
	fprintf(file_pointer, "[STAT] numero di tipi di dato scorretti:  %i\n", tipi_dato_scorretti);

	fclose(file_pointer);
	printf("[ControlloVariabile] salva_statistiche_file_esterno: statistiche salvate in '%s'\n", nome_file_output);
	return;
}


//stampa statistiche su terminale
void stampa_statistiche_su_terminale(void){
	printf("numero di variabili controllate: %i\n", variabili_controllate);
	printf("numero di errori rilevati: %i\n", errori_rilevati);
	printf("numero di nomi delle variabili non corretti: %i\n", nomi_variabili_non_corretti);
	printf("numero di tipi di dato scorretti: %i:\n", tipi_dato_scorretti);
	return;
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
	int numero_parole_riga_corrente;	//numero parole della riga corrente
	int numeroRiga=1;	//numero riga attuale

	//legge file riga per riga
	while(fgets(riga, sizeof(riga), file) != NULL){	//affinche una riga non è vuota nota: fgets() include il \n della riga

		riga[strcspn(riga, "\n")] = '\0';	//rimuove \n della riga e rimpiazza con \0

		//split della riga in parole

		char **parole = split(riga," \t", &numero_parole_riga_corrente);

		//debug
		printf("[DEBUG] riga %d\n", numeroRiga);
		for (int i = 0; i < numero_parole_riga_corrente; i++){
			printf("stringa:[%s] \n", parole[i]);  // stampa ogni stringa tra virgolette
		}

		
		//controllo riga vuota
		if (numero_parole_riga_corrente == 0) {
            printf("[ControlloVariabile] riga %d vuota, skip\n", numeroRiga);
            free(parole);
            numeroRiga++;
            continue;  // salta alla prossima riga
        }

		//controlla se è #include o //commento
		if (parole[0][0] == '#' || (parole[0][0] == '/' && strlen(parole[0]) > 1 && parole[0][1] == '/')) {
            printf("[ControlloVariabile] riga %d: commento/include, skip\n", numeroRiga);
            free(parole);
            numeroRiga++;
            continue;  // salta alla prossima riga
        }
			
		//controllo tipi variabile
		if((strcmp(parole[0],"int") == 0 || strcmp(parole[0],"long") == 0 || strcmp(parole[0], "short") == 0 )){
            printf("[ControlloVariabile] rilevato tipo intero alla riga %i\n", numeroRiga);
			variabili_controllate += 1;
		}
		else if(strcmp(parole[0],"char")==0){
			printf("[ControlloVariabile] rilevato tipo char alla riga %i\n", numeroRiga);
			variabili_controllate += 1;
		}
        else if(strcmp(parole[0],"float")==0){
            printf("[ControlloVariabile] rilevato tipo float alla riga %i\n", numeroRiga); 
			variabili_controllate += 1;          
		}
		else if(strcmp(parole[0],"bool")==0){
            printf("[ControlloVariabile] rilevato tipo bool alla riga %i\n", numeroRiga);         
			variabili_controllate += 1;                      
		}
		else{
			printf("[ControlloVariabile] rilevato errore di tipo alla riga %i\n", numeroRiga);
			errori_rilevati += 1;
		}
		//pulizia fine riga
        numeroRiga++; //passa riga successiva
		free(parole);	//libera spazio riservato dell'array parole   
		}
	
	fclose(file);
	printf("[ControlloVariabile] termine controllo variabili\n");
	return NULL;
}


