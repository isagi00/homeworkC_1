#include <stdalign.h>

/*
la funzione controlla la correttezza di:
1. dichiarazione del main + argomenti
2. dichiarazione delle strutture di controllo, es: if, else, for, while
3. dichiarazione delle variabili

*/
int main(int argc, char* argv[]){
    int a;  //commento inline
    int b; int bastardo=10;
    if(a + b < 10){
        return 1;
    }

    if (b >100){
        return b + a;
    }
    /*
la funzione controlla la correttezza di:
1. dichiarazione del main + argomenti
2. dichiarazione delle strutture di controllo, es: if, else, for, while
3. dichiarazione delle variabili

*/
    return;
}