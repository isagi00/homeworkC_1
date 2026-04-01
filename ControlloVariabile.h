#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "supporto.h"

//include guard
#ifndef CONTROLLOVARIABILE_H
#define CONTROLLOVARIABILE_H

extern int variabili_controllate;
extern int errori_rilevati;
extern int variabili_inutilizzate;
extern int nomi_variabili_non_corretti;
extern int tipi_dato_scorretti;

bool isMain(char* parola);
bool controlloTipo(char* parola);
char*  controlloVariabile(char* filename,Statistiche *stats);

//char*  controlloVariabile(char* filename, bool opzione_output, bool opzione_verbose);

List* controllaVarInutilizzate(char *nome_file_in, Statistiche *stats);

//fine include guard
#endif
