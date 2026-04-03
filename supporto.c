#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

/*funzione splitta str in un array di token.
str: stringa in input. es: ("hello world")
separatore: caratteri su cui splittare. es: (" \t;")
numeroP: numero di token all'interno dell'array.
ritorna una lista di stringhe splittate sui separatori specificati.
es: ["hello", "world"]
*/
char** split(char* str,char* separatore, int *numeroP){
	char **vettore = malloc(64*sizeof(char*));
	char *token=strtok(str,separatore);
	int i=0;
	while(token!=NULL){
		vettore[i]=token;
		i++;
		token=strtok(NULL,separatore);
	}
	*numeroP=i;
	return vettore;
}

/*
elimina spazi a destra e sinistra della stringa
- str: stringa. es: "		hello 	world		"
ritorna la stringa con spazi eliminati. 
es: "hello world"
*/
char* eliminaSpaziDxSx(char* str){
    int n = strlen(str);
	//elimina spazi dx
    for(int i = 1; i < n; i++){
        if(str[n-i] == ' ' || str[n-i] == '\t'){
            str[n-i] = '\0';
        } else {
            break;
        }
    }
	//elimina spazi sx
    while(*str == ' ' || *str == '\t'){
        str++;
    }
    return str;
}


char** split_variabile(char* str, int *numero){
	char **vettore = malloc(64*sizeof(char*));
    int i = 0;

    char* token = strtok(str, " \t");
    if(token != NULL){
        vettore[i] = eliminaSpaziDxSx(token);
        i++;
    }

    token = strtok(NULL, "=");
    if(token != NULL){
        vettore[i] =eliminaSpaziDxSx(token);
        i++;
    }

    token = strtok(NULL, "");
	if(token != NULL){
		vettore[i] = eliminaSpaziDxSx(token);
	}

    *numero = i;
    return vettore;
}



/*funzione ha come argomento un array di caratteri.
restituisce il puntatore alla prima posizione non spazio.
- str: stringa in input. es: "		hello world"
ritorna: "hello world"
*/
char *rimuoviSpaziSx(char *str){
	//str = puntatore
	while(*str == ' ' || *str == '\t'){
		str++;
	}
	return str;
}

/*controlla se la riga attuale è un commento.
restituisce true se è commento.
str: stringa in input
*/
bool controllaRigaCommento(char *str){
	char *rigapulita = rimuoviSpaziSx(str);
	
	if (rigapulita[0] == '/' && rigapulita[1] == '/'){
		return true;
	}
	return false;
}

/*controlla se la riga attuale è un #include valido o non.
restituisce true se è valido.
str: stringa in input
*/
bool controllaRigaInclude(char *str){
	char *riga_pulita = rimuoviSpaziSx(str);
	char copia[256];
	strncpy(copia, riga_pulita, sizeof(copia));

	char *token = strtok(copia, " \t");	//inserisce \0 su spazio e \t

	if (token == NULL) return false;

	//caso #include <?>
	if (strcmp(token, "#include") == 0) return true;

	//caso # 			include <?>
	if (strcmp(token, "#") == 0){
		token = strtok(NULL, " \t");	//NULL: continua dalla stringa precedente, quindi '#'
		if (token != NULL && strcmp(token, "include") == 0) return true;
	}
	return false;
}
/*
controlla se la riga attuale è una riga vuota (con solo \n).
restituisce true se è vuoto.
str: stringa in input
*/
bool controllaRigaVuota(char *str){
	char *riga_pulita = rimuoviSpaziSx(str);	//rimuove spazi davanti
	riga_pulita[strcspn(riga_pulita, "\n")] = '\0';	//sostituisce \n con \0 (per sicurezza)

	if (riga_pulita[0] == '\0'){
		return true;
	}
	return false;
}


char *pulisciNomeVariabile(char *token){
	//x=0;
	char *copia = strdup(token);

	for (int i=0; copia[i] != '\0'; i++){
		if (!isalnum(copia[i]) && copia[i] != '_'){
			copia[i] = '\0';
			break;
		}
	}
	return copia;
}

/* ricompatta una stringa rimuovendo spazi e tab.
ad esempio: "int argc,     char **      argv    [    ]"
diventa:	"intargc,char**argv[]"
- dest: destinazione della stringa compattata
- src: stringa da compattare
*/
void compatta_stringa(char* dest, const char* src){
	int j = 0;
	for(int i = 0; src[i] != '\0'; i++){
		if(src[i] != ' ' && src[i] != '\t'){
			dest[j] = src[i];
			j++;
		}
	}
	dest[j] = '\0';	//aggiungi terminatore di stringa
}














//strutture dati
//lista
typedef struct{
	void **puntatore;	//puntatore all'array di puntatori che puntano a oggetti di vario tipo
	size_t numero_elementi_attuali;	//numero di elementi attuali (slot occupati)
	size_t spazio_totale_allocato;		//numero di slot nell'array di puntatori  (slot totali)
	//nota: size_t è un tipo senza segno
}List;

//funzione che crea una lista e ritorna puntatore alla struct List.
List* list_create(){
	//allocazione memoria per la struct List
	List* nuova_lista = malloc(sizeof(List));

	//controllo se malloc ha fallito
	if (nuova_lista == NULL) {
		printf("[supporto] creazione lista fallita a causa errore di malloc\n");
		return NULL;
	}

	//impostazione valori iniziali
	nuova_lista -> numero_elementi_attuali = 0;
	nuova_lista -> spazio_totale_allocato = 10;	//10 slot di default 
	nuova_lista -> puntatore = malloc(sizeof(void*) * nuova_lista->spazio_totale_allocato);	//void* è la dimensione di un puntatore

	//controllo se malloc ha fallito per il puntatore
	if (nuova_lista->puntatore == NULL){
		printf("[supporto] creazione puntatore lista fallito a causa errore di malloc\n");
		free(nuova_lista);
		return NULL;
	}

	//printf("[supporto] lista creata correttamente\n");
	return nuova_lista;
}

//funzione aggiunge un elemento alla lista.
void list_append(List* lista, void* el){
	//controllo che la lista sia valida
	if (lista == NULL) {
		printf("[supporto] list_append : lista vuota o non valida\n");
		return;
	}

	//assegna l'elemento nella lista
	if (lista->numero_elementi_attuali < lista->spazio_totale_allocato){
		lista->puntatore[lista->numero_elementi_attuali] = el;
		lista->numero_elementi_attuali += 1;
		//printf("[supporto] list_append: elemento aggiunto correttamente \n");
	}
	else{
		printf("[supporto] list_append: lista piena\n");
		return;
	}
}


//funzione ritorna un oggetto della lista
void* list_get(List* list, size_t indice){
	//controllo lista valida
	if (list == NULL){
		printf("[supporto] list_get: lista non valida\n");
		return NULL;
	}

	//controllo out of range index
	if (indice >= list->numero_elementi_attuali){
		printf("[support] list_get: indice out of range.\nindice = %zu\nelementi lista = %zu\n", indice, list->numero_elementi_attuali);
		return NULL;
	}

	//ritorna puntatore all'oggetto
	return list->puntatore[indice];
}


void list_free(List* list){
	if (list == NULL){
		printf("[supporto] list_free: lista non valida. \n");
		return;
	}

	//free array interno
	if (list->puntatore != NULL) {
		free(list->puntatore);
	}

	//free struct stessa
	free(list);

	//printf("[supporto] list_free: lista liberata correttamente\n");
	return;
}



//variabile

typedef struct {
	char* nome; 	//nome della variabile
	int riga_dichiarata; 	//riga in cui è stata dichiarata la variabile
	bool usata; 	//flag, true se la variabile è stata usata	
} Variabile;


//statistiche
typedef struct{
	int variabili_controllate;
	int errori_rilevati;
	int variabili_inutilizzate;
	int nomi_variabili_non_corretti;
	int tipi_dato_scorretti;
} Statistiche;

int conta_var_inutilizzate(List *vars){
	//lista vuota
	if (vars->numero_elementi_attuali==0){
		return 0;
	}
	//conta var inutilizzate
	int count = 0;
	for (int i=0; i < vars->numero_elementi_attuali; i++){
		Variabile *var = list_get(vars, i);
		if (var->usata == false){
			count++;
		}
	}
	return count;
}



//salva statistiche su file esterno
void salva_statistiche_file_esterno(char *nome_file_output, Statistiche *stats, List *var_inutilizzate){
	FILE *file_pointer = fopen(nome_file_output, "w"); //puntatore a file esterno

	if (file_pointer == NULL) {
		printf("[STAT] salva_statistiche_file_esterno : Errore apertura file output\n");
		return;
	}

	stats->variabili_inutilizzate = conta_var_inutilizzate(var_inutilizzate);

	//scrittura su file output
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di variabili controllate: %i\n", stats->variabili_controllate);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di errori rilevati: %i\n", stats->errori_rilevati);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di variabili non utilizzati: %i\n", stats->variabili_inutilizzate);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di nomi delle variabili non corretti: %i\n", stats->nomi_variabili_non_corretti);
	fprintf(file_pointer, "[STAT] salva_statistiche_file_esterno: numero di tipi di dato scorretti:  %i\n", stats->tipi_dato_scorretti);

	//scrittura nome variabili non usate
	for (int i = 0; i < var_inutilizzate->numero_elementi_attuali; i++){
		Variabile *var = list_get(var_inutilizzate, i);
		if (var->usata == false){
			fprintf(file_pointer,"[STAT] salva_statistiche_file_esterno: Variabile '%s' inutilizzata alla riga %i\n", var->nome, var->riga_dichiarata);
		}
	}

	fclose(file_pointer);
	printf("[STAT] salva_statistiche_file_esterno: statistiche salvate in '%s'\n", nome_file_output);
	return;
}


//stampa statistiche su terminale
void stampa_statistiche_su_terminale(Statistiche *stats, List *var_inutilizzate){

	stats->variabili_inutilizzate = conta_var_inutilizzate(var_inutilizzate);

	printf("[STAT] stampa_statistiche_su_terminale: numero di variabili controllate: %i\n", stats->variabili_controllate);
	printf("[STAT] stampa_statistiche_su_terminale: numero di errori rilevati: %i\n", stats->errori_rilevati);
	printf("[STAT] stampa_statistiche_su_terminale: numero di variabilio non utilizzati: %i\n", stats->variabili_inutilizzate);
	printf("[STAT] stampa_statistiche_su_terminale: numero di nomi delle variabili non corretti: %i\n", stats->nomi_variabili_non_corretti);
	printf("[STAT] stampa_statistiche_su_terminale: numero di tipi di dato scorretti: %i:\n", stats->tipi_dato_scorretti);

	//scrittura nome variabili non usate
	for (int i = 0; i < var_inutilizzate->numero_elementi_attuali; i++){
		Variabile *var = list_get(var_inutilizzate, i);
		if (var->usata == false){
			printf("[STAT]stampa_statistiche_su_terminale: variabile '%s' inutilizzata alla riga %i\n", var->nome, var->riga_dichiarata);
		}
	}
	return;
}

