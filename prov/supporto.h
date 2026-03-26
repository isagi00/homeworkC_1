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

