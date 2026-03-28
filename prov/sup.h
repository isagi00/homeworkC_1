#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char** split(char* separaS,char* separatore, int *numeroP){
        char **vettore = malloc(64*sizeof(char*));
        char *token=strtok(separaS,separatore);
        int i=0;
        while(token!=NULL){
                vettore[i]=token;
                i++;
                token=strtok(NULL,separatore);
        }
        *numeroP=i;
        return vettore;
}

