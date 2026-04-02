#include <stddef.h>
#include <stdbool.h>


//include guard
#ifndef SUPPORTO_H
#define SUPPORTO_H


//funzioni di supporto
char** split(char* separaS,char* separatore, int *numeroP);
char** split_variabile(char* str, int *numero);
char* eliminaSpaziDxSx(char* str);
char *rimuoviSpaziSx(char *str);
bool controllaRigaCommento(char *str);
bool controllaRigaInclude(char *str);
bool controllaRigaVuota(char *str);
char *pulisciNomeVariabile(char *token);
void compatta_stringa(char* dest, const char* src);





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

typedef struct {
	char* nome; 	//nome della variabile
	int riga_dichiarata; 	//riga in cui è stata dichiarata la variabile
	bool usata; 	//flag, true se la variabile è stata usata	
} Variabile;


typedef struct{
	int variabili_controllate;
	int errori_rilevati;
	int variabili_inutilizzate;
	int nomi_variabili_non_corretti;
	int tipi_dato_scorretti;
} Statistiche;

int conta_var_inutilizzate(List *vars);
void salva_statistiche_file_esterno(char *nome_file_output, Statistiche *stats, List *var_inutilizzate);
void stampa_statistiche_su_terminale(Statistiche *stats, List *var_inutilizzate);

#endif
//fine include guard
