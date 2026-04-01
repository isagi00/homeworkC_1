#include <stdio.h>
#include "sup.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
bool controlloTipo(char* parola){
	if(strcmp(parola,"char")==0){
		return true;
	}
	else if(strcmp(parola,"int")==0 || strcmp(parola,"long")==0 || strcmp(parola,"short")==0){
        return true;
    }
	else if(strcmp(parola,"float")==0 || strcmp(parola,"double")==0){
        return true;
    }else if(strcmp(parola,"bool")==0){
        return true;
    }
    return false;
}
bool isMain(char* riga) {
    int numeroParola;
    char** parola = split(riga, "()", &numeroParola);
    
    if(numeroParola < 1){
        free(parola);
        return false;
    }

    int p;
    bool controlloPrimaParte = false;
    char** primaParte = split(parola[0], " \t", &p);
    if(p >= 2 && strcmp(primaParte[0], "int") == 0 && strcmp(primaParte[1], "main") == 0){
        controlloPrimaParte = true;
    }
    free(primaParte);

    if(numeroParola > 1 && strlen(parola[1]) > 0){
        int p1;
        bool controlloSecondaParte = false;
        char** secondaParte = split(parola[1], " ,\t", &p1);
        if(p1 >= 4 && strcmp(secondaParte[0], "int") == 0 && strcmp(secondaParte[1], "argc") == 0
        && strcmp(secondaParte[2], "char") == 0 && strcmp(secondaParte[3], "*argv[]") == 0)
        {
            controlloSecondaParte = true;
        }
        free(secondaParte);

        if(controlloPrimaParte && controlloSecondaParte){
            free(parola);
            return true;
        }
    } else {
        if(controlloPrimaParte){
            free(parola);
            return true;
        }
    }
    free(parola);
    return false;
}
bool isFunzione(char* parola){
    char* parole_da_split = strdup(parola); 
    
    char* parole_splitate_tipo = strtok(parole_da_split, " ");
    char* parole_splitate_nome = NULL;  
    
    if(parole_splitate_tipo != NULL){
        parole_splitate_nome = strtok(NULL, "");
    } else {
        free(parole_da_split);
        return false;
    }

    if(!controlloTipo(parole_splitate_tipo)){
        free(parole_da_split);
        return false;
    }
    
    if(parole_splitate_nome != NULL){
        int i = strlen(parole_splitate_nome);
        bool esiste_aperta = false; 

        for(int j = 0; j < i - 1; j++){
            char c = parole_splitate_nome[j];
            if(j == 0 && (c == '*' || (c == '*' && parole_splitate_nome[1] == '*'))){
                continue;  
            }
            if(c == '('){
                if(!esiste_aperta){
                    esiste_aperta = true;
                } else {
                    free(parole_da_split);
                    return false;  // doppia (
                }
            } else if(!(isalnum(c) || c == '_')){
                if(!esiste_aperta){
                    free(parole_da_split);
                    return false;  
                }
            }
        }
        if(parole_splitate_nome[i - 1] != ')'){
            free(parole_da_split);
            return false;
        }
        free(parole_da_split);
        return true;
    }

    free(parole_da_split);
    return false;
}

bool controllaVar(char* parola, char* tipo){
    int n;
    char** sep = split(parola, "=", &n);  
    if(n != 2){                          
        free(sep);
        return false;
    }

    char* valore = sep[1];

    if(strcmp(tipo,"char")==0){      
        int n1;
        char** c = split(valore, "'", &n1);
        bool ok = (n1 == 3);            
        free(c);
        free(sep);
        return ok;
    }
    else if(strcmp(tipo,"int")==0 || strcmp(tipo,"long")==0 || strcmp(tipo,"short")==0){ 
        for(int i = 0; i < (int)strlen(valore); i++){
            if(!isdigit(valore[i])){
                free(sep);
                return false;
            }
        }
        free(sep);
        return true;
    }
    else if(strcmp(tipo,"float")==0 || strcmp(tipo,"double")==0){ 
        bool haPunto = false;            
        for(int i = 0; i < (int)strlen(valore); i++){
            if(valore[i] == '.'){
                if(haPunto){ free(sep); return false; }
                haPunto = true;
            } else if(!isdigit(valore[i])){
                free(sep);
                return false;
            }
        }
        free(sep);
        return true;
    }
    else if(strcmp(tipo,"bool")==0){     
        if(strcmp(valore,"true")==0  || strcmp(valore,"false")==0 || strcmp(valore,"1")==0 || strcmp(valore,"0")==0){
            free(sep);
            return true;
        }
        free(sep);
        return false;
    }
    free(sep);
    return false;
}

int main(){
    char* p1="maskja='a'";
    char* p2="char";

    bool p=controllaVar(p1,p2);
    char mp[]="int m()";
    /*bool n1=isMain(mp);
    if(n1){
        printf("è main");
    }else{
        printf("non è main");
    }*/
   bool n1=isFunzione(mp);
    printf("%d",p);
}