#include <stddef.h>


//include guard
#ifndef SUPPORTO_H
#define SUPPORTO_H


//funzioni di supporto
char** split(char* separaS, int *numeroP);







//struct List
typedef struct{
	void **puntatore;	//puntatore all'array di puntatori che puntano a oggetti di vario tipo
	size_t numero_elementi_attuali;	//numero di elementi attuali (slot occupati)
	size_t spazio_totale_allocato;		//numero di slot nell'array di puntatori  (slot totali)
	//nota: size_t è un tipo senza segno
}List;

List* list_create(void); //nessun parametro necessario
void list_append(List* lista, void* el);
void* list_get(List* list, size_t indice);
void list_free(List* list);


#endif
//fine include guard