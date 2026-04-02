
#include <stdio.h>
#include "../supporto.h"
#include "../ControlloVariabile.h"



int main(){
// Tutti questi devono restituire true (1)
printf("test1: %d\n", controlloArgomentiMain("void"));
printf("test2: %d\n", controlloArgomentiMain("int argc, char*argv[]"));
printf("test3: %d\n", controlloArgomentiMain("int num, char * params[]"));
printf("test4: %d\n",controlloArgomentiMain("int x, char**y[]"));
printf("test5: %d\n",controlloArgomentiMain("int a, char * * b [ ]"));

// Tutti questi devono restituire false (0)
printf("test6: %d\n",controlloArgomentiMain("c har * argv[]"));      // "char" spezzato
printf("test7: %d\n",controlloArgomentiMain("int argc, char*argv")); // manca []
printf("test8: %d\n",controlloArgomentiMain("int argc, int*argv[]"));// tipo sbagliato
printf("test9: %d\n",controlloArgomentiMain("int argc"));            // manca secondo argomento
printf("test10: %d\n",controlloArgomentiMain(""));                    // stringa vuota
}