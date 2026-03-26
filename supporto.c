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

//ritorna puntatore alla struct List
List* crea_lista(){
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

	printf("[supporto] lista creata correttamente");
	return nuova_lista;
}
