#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

const char* qualificatori[] = {"const", "volatile", "restrict", "_Atomic", NULL};

const char* storage[] = {"auto", "static", "extern", "register", NULL};





_Bool controllaLongShort_tre_sessione(char** parti_var) {
    return (
		// casi con "long" prima
    	strcmp(parti_var[0], "long") == 0 && (
    	// "long long a"
    	(strcmp(parti_var[1], "long") == 0 ) ||
    	// "long int a"
    	(strcmp(parti_var[1], "int") == 0 ) ||
    	// "long double a"
        (strcmp(parti_var[1], "double") == 0 ) 
    	) ||

    	// "unsigned long a"
    	(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 ) ||
    	// "signed long a"
		(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 ) ||

    	// casi con "short" prima
    	strcmp(parti_var[0], "short") == 0 && (
    	// "short int a"
        (strcmp(parti_var[1], "int") == 0 )) ||

    	// "unsigned short a"
    	(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "short") == 0 ) ||
    	// "signed short a"
    	(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "short") == 0 ) ||

		// casi con char
		(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "char") == 0 ) ||
		// "signed char a"
    	(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "char") == 0 ) ||

		// casi con int
		(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "int") == 0 ) ||
		// "signed char a"
    	(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "int") == 0 )
	);
}

/*
[tipo tipo tipo] nome
*/
_Bool controllaLongShort_quattro_sessione(char** parti_var) {
	return (
        // "long long int a"
        (strcmp(parti_var[0], "long") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 ) ||
        // "long long a" con unsigned/signed
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 ) ||
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 ) ||
        // "unsigned long int a"
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 ) ||
        // "signed long int a"
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 ) ||
        // "unsigned short int a"
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "short") == 0 && strcmp(parti_var[2], "int") == 0 ) ||
        // "signed short int a"
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "short") == 0 && strcmp(parti_var[2], "int") == 0 ) ||
        // "long int a" con signed
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 )
	);
}

//max numero di tipo dic
_Bool controllaLongShort_cinque_sessione(char** parti_var) {
    return (
        // "unsigned long long int a"
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 && strcmp(parti_var[3], "int") == 0 ) ||
        // "signed long long int a"
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 && strcmp(parti_var[3], "int") == 0 )
    );
}





char* eliminaSpaziDxSx(char* str){
	if (!str) return NULL;

    int n = strlen(str);
	//elimina spazi dx
    for(int i = 1; i < n; i++){
        if(str[n-i] == ' ' || str[n-i] == '\t'){
            str[n-i] = '\0';
        } else {
            break;
        }
    }
	//elimina spazi sx
    while(*str == ' ' || *str == '\t'){
        str++;
    }
    return str;
}





_Bool ricerca_array(char *str, const char *arr[]){
	if(!str||!arr){return 	0;}


	for (int i = 0; arr[i] != NULL; i++){
		if (strcmp(arr[i], str) == 0){
			return 1;
		}
	}
	return 0;
	
}





char* el(char* str) {
    int number = strlen(str);
    static char buffer[1024];
    static char buffer_ultim_pulito_pulito_super_pulito_overturned[1024];
    _Bool start_delete_space = 0;
    int position_for_buffer = 0;

    for(int i = 0; i < number; i++) {
        if(str[i] == ',' || str[i] == '=' || str[i] == '[' || str[i] == ']' || str[i] == '*') {
            start_delete_space = 1;
        } else if(str[i] != ' ' && str[i] != '\t') {
            start_delete_space = 0;
        }
        if(start_delete_space == 1 && (str[i] == ' ' || str[i] == '\t')) {
            continue;
        }
        buffer[position_for_buffer] = str[i];
        position_for_buffer++;
    }
    buffer[position_for_buffer] = '\0';
    position_for_buffer = 0;
    start_delete_space = 0; 

    int len_buffer = strlen(buffer);
    for(int i = len_buffer - 1; i >= 0; i--) {

        if(buffer[i] == ',' || buffer[i] == '=') {
            start_delete_space = 1;
        }
        else if(buffer[i] != ' ' && buffer[i] != '\t') {
            start_delete_space = 0;
        }

        if(start_delete_space == 1 && (buffer[i] == ' ' || buffer[i] == '\t')) {
            continue;
        }

        buffer_ultim_pulito_pulito_super_pulito_overturned[position_for_buffer] = buffer[i];
        position_for_buffer++;
    }
    buffer_ultim_pulito_pulito_super_pulito_overturned[position_for_buffer] = '\0';

    int len = position_for_buffer;
    for(int i = 0; i < len / 2; i++) {
        char tmp = buffer_ultim_pulito_pulito_super_pulito_overturned[i];
        buffer_ultim_pulito_pulito_super_pulito_overturned[i] = buffer_ultim_pulito_pulito_super_pulito_overturned[len - 1 - i];
        buffer_ultim_pulito_pulito_super_pulito_overturned[len - 1 - i] = tmp;
    }   

    
    return buffer_ultim_pulito_pulito_super_pulito_overturned;
}





char** split(char* str,char* separatore, int *numeroP){
	char **vettore = malloc(64*sizeof(char*));// aggiungi if malloc

	char *token=strtok(str,separatore);
	int i=0;
	while(token!=NULL){
		vettore[i]=token;
		i++;
		token=strtok(NULL,separatore);
	}
	*numeroP=i;
	return vettore;
}





_Bool controlloTipo(char* parola){
	//caso: 'char'
	if(strcmp(parola,"char")==0){
		return 1;
	}
	//casi: 'int' 'long' 'short
	else if(strcmp(parola,"int")==0 || strcmp(parola,"long")==0 || strcmp(parola,"short")==0){
        return 1;
    }
	//casi: 'float', 'double'
	else if(strcmp(parola,"float")==0 || strcmp(parola,"double")==0){
        return 1;
    }
	//caso: 'bool'
	else if(strcmp(parola,"bool")==0){
        return 1;
    }
    return 0;
	}





_Bool controllaNome(char* parola){
	if (parola == NULL || strlen(parola) == 0){
		printf("[controlloNome] nome vuoto o NULL. \n");
		return 0;
	}

	char* copia = strdup(parola);
	if (copia == NULL) return 0;
	char* pulito = eliminaSpaziDxSx(copia);
    int n = strlen(pulito);

    _Bool open_square_brackets = 0;
    _Bool close_square_brackets = 0;


	if(n == 0){
		printf("[controllaNome] nome vuoto dopo pulizia spazi dx,sx. \n");
		return 0;
	}
    char buffer[1024];
    int numero_buffer = 0;
    _Bool not_is_integer = 0;
    _Bool continue_nome = 1;
    


	for (int i = 0; i < n; i++){

        if(pulito[i] == '['){
            continue_nome = 0;
            open_square_brackets = 1;
            continue;
        }

        if(pulito[i] == ']'){
            open_square_brackets = 0;
            close_square_brackets = 1;
        }

        if(open_square_brackets){
            if(!isdigit(pulito[i])){
                not_is_integer++;
            }
            buffer[numero_buffer++] = pulito[i];
            continue;
        }

        if(close_square_brackets){
            buffer[numero_buffer] = '\0';

            if(!controllaNome(buffer) && not_is_integer){
                free(copia);
                return 0;
            }   

            for(int k = 0; k < numero_buffer; k++){
                buffer[k] = '\0';
            }

            numero_buffer = 0;
            not_is_integer = 0;
            close_square_brackets = 0;
            continue;
        }
        if(continue_nome){
            //primo carattere puo essere solo lettera o '_'
		    if (i == 0){
			    if(!isalpha(pulito[i])  && pulito[i] != '_'){ 
				    printf("[controllaNome] il nome della variabile non può iniziare con '%c'. \n", pulito[i]);
				    free(copia);
				    return 0;
			    }
		    }
		//gli altri caratteri possono essere numeri, lettere o underscore
		    else {
			    if(!isalnum(pulito[i]) && pulito[i] != '_'){
				    printf("[controllaNome] carattere '%c' non può essere nome variabile. \n", pulito[i]);
				    free(copia);
				    return 0;
			    }
		    }
	    }else {
            if(open_square_brackets == 0 && close_square_brackets == 0){
                free(copia);
                return 0;
            }
        }
	
	//controlla se è keyword
	/*if (is_keyword(pulito)) {
		printf("[controllaNome] '%s' è una parola chiave riservata.\n", pulito);
        free(copia);
        return false;
	}*/

        }
		
	free(copia);
	return 1;

}





_Bool controlla_correttezza_nome(char* nome_valore){
    int numero_variabile;
    char* copy_nome_valore = strdup(nome_valore);
    char** variabili_separati = split(copy_nome_valore, ",", &numero_variabile);

    

    for (int i=0; i<numero_variabile; i++){
        int numero;
        char** variabile_valore_separato = split(variabili_separati[i],"=",&numero);
        _Bool is_name_last = controllaNome(variabile_valore_separato[0]);
        if(!is_name_last) {
            free(variabile_valore_separato);
            free(variabili_separati);
            free(copy_nome_valore);
            return 0;
        }
        _Bool is_name;
        for (int j=1; j<numero; j++){
            is_name = controllaNome(variabile_valore_separato[j]); // controllaNome -> controlla se è stato dichiarato 
            if(is_name_last){
                is_name_last = is_name;
            }
            else{
                
                return 0;
            }
        }

    }
    free(variabili_separati);
    free(copy_nome_valore);
    return 1;
}





_Bool controllaDichiarazioneVariabile(char* stringa){
    if(!stringa) return 0;

    int numero_parti;
    char* copy_stringa = strdup(stringa);

    char** parti_variabile = split(el(copy_stringa)," \t",&numero_parti);
    char** parti_variabile_copy = parti_variabile;

    _Bool ha_storage = 0;
    char* stor = NULL;
    _Bool ha_qualific = 0;
    char* qualific = NULL;

    while ((!ha_storage && ricerca_array(parti_variabile[0], storage) || (!ha_qualific && ricerca_array(parti_variabile[0],qualificatori))))
    {

        if(!ha_storage && ricerca_array(parti_variabile[0],storage)){
            ha_storage = 1;
            stor = strdup(parti_variabile[0]);
            parti_variabile++;
            numero_parti--;
        }

        if(!ha_qualific && ricerca_array(parti_variabile[0],qualificatori)){
            ha_qualific = 1;
            qualific = strdup(parti_variabile[0]);
            numero_parti++;
            numero_parti--;
        }
    }

    if (!parti_variabile)
    {
        free(copy_stringa);
        return 0;
    }

//--------------------------------------------------------------------------------------------//

    _Bool risultato_tipo = 1;
    _Bool risultato_nome = controlla_correttezza_nome(parti_variabile[numero_parti-1]);
    if(numero_parti <= 1){
        risultato_tipo = 0;
    }
    else if(numero_parti == 2){
        risultato_tipo = controlloTipo(parti_variabile[0]);
    }
    else if(numero_parti == 3){
        risultato_tipo = controllaLongShort_tre_sessione(parti_variabile);
    }
    else if(numero_parti == 4){
        risultato_tipo = controllaLongShort_quattro_sessione(parti_variabile);
    }
    else if(numero_parti == 5){
        risultato_tipo = controllaLongShort_cinque_sessione(parti_variabile);
    }
    else {
        risultato_tipo = 0;
    }
    
    return (risultato_nome && risultato_tipo);
}


int main() {
    int i[4];
    int m[]={1,2};
    
    char l[] = "    unsigned long int a[12][2][asjk[sasa[sasa[1]]]]";
    printf("%d\n",controllaDichiarazioneVariabile(l));

}