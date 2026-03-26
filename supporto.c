#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** split(char* separaS, int *numeroP){
	char **vettore = malloc(64*sizeof(char*));
	char *token=strtok(separaS," ");
	int i=0;
	while(token!=NULL){
		vettore[i]=token;
		i++;
		token=strtok(NULL," ");
	}
	*numeroP=i;
	return vettore;
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

	printf("[supporto] lista creata correttamente\n");
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
		printf("[supporto] list_append: elemento aggiunto correttamente \n");
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

	printf("[supporto] list_free: lista liberata correttamente\n");
	return;
}


