#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
    //opzioni : 
    //1. -i oppure --in
    //2. -o oppure --out
    //3. -v oppure --verbose
    char nome_file[256];

    //controllo argomenti minimi
    if (argc < 2 || argc > 6) {    //1: main, 2: -i, 3:nomefile.c
         printf("[main]: utilizzo minimo: -i fileinput.c\n ");
         printf("tutte le opzioni:\n");
         printf("(1) -i --in fileinput.c : specifica il nome del file .c\n");
         printf("(2) -o --out fileoutput : di default stampa su terminale, specificando -o nomefile crea un file con le statistiche.\n");
         printf("(3) -v --verbose : visualizza sul terminale le statistiche.\n ");
         return 0;
        }
    
    //controllo input file
    if (strcmp(argv[1], "-i") == 0 || strcmp(argv[1],"--i") == 0){    //strncmp() ritorna 0 se le 2 stringhe sono uguali
        strcpy(nome_file, argv[2]);
        //....


    }
    


    
    

}


