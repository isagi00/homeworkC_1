#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "supporto.h"


// const char* tipi_base[] = {"int", "float", "double", "char", "long", "bool", NULL};
const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while",
    NULL
};

/*stampa le stringhe dell'array dato. usato rigorosamente dopo il split. 
- parole: array di stringhe. es : ["hello", "world"]
- numero_parole: numero di stringhe all'interno dell'array.
*/
void stampaParoleSplit(char **parole, int numero_parole) {
    printf("Contenuto di parole_split_pv (numero_parole = %d):\n", numero_parole);
    for (int i = 0; i < numero_parole; i++) {
        printf("[%d]: '%s'\n", i, parole[i]);
    }
}









/*
data una parola, ritorna se la parola è equivalente a un tipo valido.
- parola: stringa da controllare. es: "hello world"
ritorna true se la parola data è una dichiarazione di tipo corretto.
*/
bool controlloTipo(char* parola){
	//caso: 'char'
	if(strcmp(parola,"char")==0){
		return true;
	}
	//casi: 'int' 'long' 'short
	else if(strcmp(parola,"int")==0 || strcmp(parola,"long")==0 || strcmp(parola,"short")==0){
        return true;
    }
	//casi: 'float', 'double'
	else if(strcmp(parola,"float")==0 || strcmp(parola,"double")==0){
        return true;
    }
	//caso: 'bool'
	else if(strcmp(parola,"bool")==0){
        return true;
    }
	//gestisci casi per tipi definiti dall'utente


	printf("[Errore dichiarazione tipo]");
    return false;
}

/*determina se la stringa è una dichiarazione di una funzione. 

*/
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

/*
controlla se la stringa corrente è una keyword.
-str: stringa
ritorna true se è una keyword.
*/
bool is_keyword(char* str){
	//check tipi
	for (int i = 0; tipi_base[i] != NULL; i++){
		if (strcmp(str,tipi_base[i]) == 0){
			return true;
		}
	}
	//check su altri keyword
	for (int j = 0; keywords[j] != NULL; j++){
		if(strcmp(str, keywords[j]) == 0){
			return true;
		}
	}
	return false;
}


/*
controlla se il nome della variabile dichirata è consentita.
- parola: nome della variabile : es. 'var'
ritorna true se il nome della variabile dichiarata è valida.
*/
bool controllaNome(char* parola){
	if (parola == NULL || strlen(parola) == 0){
		printf("[controlloNome] nome vuoto o NULL. \n");
		return false;
	}

	char* copia = strdup(parola);
	if (copia == NULL) return false;
	char* pulito = eliminaSpaziDxSx(copia);
    int n = strlen(pulito);

	if(n == 0){
		printf("[controllaNome] nome vuoto dopo pulizia spazi dx,sx. \n");
		return false;
	}
    
	for (int i = 0; i<n; i++){
		//primo carattere puo essere solo lettera o '_'
		if (i == 0){
			if(!isalpha(pulito[i])  && pulito[i] != '_'){ 
				printf("[controllaNome] il nome della variabile non può iniziare con '%c'. \n", pulito[i]);
				free(copia);
				return false;
			}
		}
		//gli altri caratteri possono essere numeri, lettere o underscore
		else {
			if(!isalnum(pulito[i]) && pulito[i] != '_'){
				printf("[controllaNome] carattere '%c' non può essere nome variabile. \n", pulito[i]);
				free(copia);
				return false;
			}
		}
	}
	
	//controlla se è keyword
	if (is_keyword(pulito)) {
		printf("[controllaNome] '%s' è una parola chiave riservata.\n", pulito);
        free(copia);
        return false;
	}

	free(copia);
	return true;

}

/*
funzione controlla la dichiarazione corretta degli argomenti del main().
gestisce anche il caso in cui si ha come argomento 'void'
- args: argomenti del main. es: "int argc, char*argv[]"
ritorna true se gliu argomenti sono corretti.
*/

bool controlloArgomentiMain(char* args){
	if (args == NULL) return false;

	//copia modificabile di args
	char* copia = strdup(args);		//free() prima di return
	if (!copia) return false;

	//elimina spazi avanti e dietro
	char* copia_pulita = eliminaSpaziDxSx(copia);

	//caso: argomenti vuoti, ovvero 'int main()'
	if(strlen(copia_pulita) == 0){
		free(copia);
		return true;
	}

	//caso: 'void'
	if(strcmp(copia_pulita, "void") == 0){
		free(copia);
		return true;
	}

	//caso: 'int <nome>, char [*][*] <nome>[]'
	int numero_parti;
	char** parti = split(copia_pulita, ",", &numero_parti);	//free() prima di return
	if (numero_parti != 2){
		free(copia);
		free(parti);
		printf("[controlloArgomentiMain] numero di argomenti non valido.\n");
		return false;
	}

	//verifica di 'int <nome>'
	char* prima_parte = eliminaSpaziDxSx(parti[0]);
	int len_prima_parte = strlen(prima_parte);
	int numero_token_prima_parte;
	char** tokens_prima_parte = split(prima_parte, " \t", &numero_token_prima_parte);
	//deve avere esattamente 2 token: 'int' + 'nome_valido'
	bool ok_primaparte = (numero_token_prima_parte == 2 &&
						  strcmp(tokens_prima_parte[0],"int") == 0 &&
						  controllaNome(tokens_prima_parte[1]));
	free(tokens_prima_parte);
	if (!ok_primaparte){
		free(copia);
		free(parti);
		printf("[controlloArgomentiMain] dichiarazione 'int <nome>' non valido. \n");
		return false;
	}

	//verifica di 'char[*][*] <nome>[]
	char* seconda_parte = eliminaSpaziDxSx(parti[1]);
	int len_seconda_parte = strlen(seconda_parte);
	int pos = 0;
	//il primo token deve essere necessariamente 'char'
	while(pos<len_seconda_parte && isspace(seconda_parte[pos])) pos++;//salta spazi iniziali
	if(strncmp(&seconda_parte[pos], "char", 4) != 0){
		free(copia);
		free(parti);
		printf("[controlloArgomentiMain] dichiarazione 'char' invalido. \n");
		return false;
	}
	pos += 4;

	//conta asterischi
	int asterischi = 0;
	while (pos < len_seconda_parte){
		if (seconda_parte[pos] == '*'){
			asterischi++;
			pos++;
		}
		else if(isspace(seconda_parte[pos])){
			pos++;
		}
		else break;	//fine asterischi
	}
	//deve esserci almeno un asterisco, massimo 2
	if (asterischi < 1 || asterischi > 2){
		free(parti);
		free(copia);
		printf("[controlloArgomentiMain] asterischi mancanti oppure numero di asterischi eccessivo. \n");
		return false;
	}


	//leggi il nome della variabile
	while (pos<len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salta spazi

	int inizio_nome = pos;
	while (pos < len_seconda_parte && (isalnum(seconda_parte[pos]) || seconda_parte[pos] == '_')){
		pos++;
	}
	int len_nome = pos - inizio_nome;
	if (len_nome == 0){
		free(parti);
		free(copia);
		printf("[controlloArgomentiMain] specifica un nome 'char <nome>'.\n");
		return false;
	}
	//estrai e valida nome
	char* nome_var = strndup(&seconda_parte[inizio_nome], len_nome);
	if (!nome_var || !controllaNome(nome_var)){
		free(nome_var);
		free(copia);
		free(parti);
		printf("[controlloArgomentiMain] nome inserito in 'char <nome' invalido. \n");
		return false;
	}
	free(nome_var);

	//verifica del '[]' finale
	while (pos < len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salti spazi
	if (pos >= len_seconda_parte || seconda_parte[pos] != '['){	//carattere in secondaparte[pos] deve essere '['
		free(copia);
		free(parti);
		printf("[controlloArgomentiMain] manca il '[' dopo argv. \n");
		return false;
	}
	pos++;

	while (pos < len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salta spazi
	if (pos >= len_seconda_parte || seconda_parte[pos] != ']'){	//carattere in secondaparte[pos] deve essere ']'
		free(copia);
		free(parti);
		printf("[controlloArgomentiMain] manca il ']' dopo argv. \n");
		return false;
	}
	pos++;

	//non deve rimanere nulla
	while(pos<len_seconda_parte && isspace(seconda_parte[pos])) pos++;	//salta spazi

	bool ok_secondaparte = (pos == len_seconda_parte);

	free(copia);
	free(parti);
	return ok_secondaparte;	//ok_primaparte verificato prima.
}




_Bool isInt(char* valore, bool isSigned){
    int i = 0;
    // salta il segno se c'è
    if(valore[0] == '-' || valore[0] == '+' && isSigned) {
        i = 1;
    }
	else{
		return 0;
	}
    
    // se è solo "+" o "-" 
    if(valore[i] == '\0'){
		return 0;
	}
    
    // controlla che tutti i caratteri restanti siano cifre
    while(valore[i] != '\0') {
        if(!isdigit(valore[i])){
			return 0;
		}
        i++;
    }
    
    return 1;
}

bool isChar(char* valore, bool isSigned){
	bool char_lettera_ok = false;
	bool char_numero = false; 
	bool char_numero_ok =false;
	if(strlen(valore)<=3 && strlen(valore)>=2 && valore[0]=='\'' && valore[strlen(valore)-1]=='\''){
		char_lettera_ok = true;
	}

    if(isInt(valore, isSigned)){
		char_numero = true;
	}

	if(char_numero){
		int n=atoi(valore);	//cast "42" -> 42 ?
		if(isSigned){
			if(n >= -128 && n <= 127){
				char_numero_ok = true;
			}
		}
		else {
			if(n >= 0 && n <= 255){
				char_numero_ok = true;
			}
		}
	}
	return(char_lettera_ok || char_numero_ok);
}


/*
controlla se il valore assegnato alla variabile è valido o non.
-valore: stringa che rappresente il valore assegnato alla variabile. es:"10"
-tipo: tipo di dato dichiarato per la variabile. es: "int"
ritorna true se il valore assegnato è valido
@TODO:
unsigned int x = 42;
unsigned long x = 100000UL;
unsigned long long x = 9000000000ULL;
long long x = -9000000000LL;
long double x = 3.14159265358979323846L;
L è opzionale
*/
bool controlloValoreAssegnato(char* valore,char* tipo){
	// printf("[controlloValoreAssegnato] valore : '%s'\n", valore);
	//char
	if(strcmp(tipo,"char")==0){      
		return isChar(valore, true);
    }
	//int, long, short
	else if(strcmp(tipo,"int")==0 || strcmp(tipo,"long")==0 || strcmp(tipo,"short")==0){ 

        for(int i = 0; i <strlen(valore); i++){
			if(i== 0 && valore[i] == '"') return false;	//se inizia con " sicuramente è stringa, salta
			if(i == 0 && (valore[i] == '+' || valore[i] == '-')) continue; // segno ok
       	 	if(valore[i] == ' ' || valore[i] == '\t') continue;	//salta spazi
			/*
			l'assegnazione 'a = b + c' deve ritornare corretto, se b e c sono variabili interi. 
			altrimenti ritornerebbe false scorrettaemnte
			*/
       		// if(!digit(valore[i])) return false;	//non è numero
       		// if(isalpha(valore[i])) return false; // no lettere, solo cifre

            // if( !(isalnum(valore[i]) ||valore[i]=='+'||valore[i]=='-'||valore[i]==' '|| valore[i]=='\t') ){
            //     return false;
            // }
        }
        return true;
    }
	//float, double
	else if(strcmp(tipo,"float")==0 || strcmp(tipo,"double")==0){
    	bool haPunto = false;
    	bool haE = false;
    	int len = strlen(valore);

    	for(int i = 0; i < len; i++){
        	if(valore[i] == '.'){
            	if(haPunto || haE) return false;
            	haPunto = true;
        	}
        	else if(valore[i] == 'e' || valore[i] == 'E'){
            	if(haE) return false; 
            	haE = true;
        	}
        	else if(valore[i] == '+' || valore[i] == '-'){
            	if(i != 0){
					return false; 
				} 
        	}	
        	else if(valore[i] == ' ' || valore[i] == '\t'){
            	continue; 
        	}
        	else if(!isdigit(valore[i])){
            	return false;
        	}
			//tutto ok ritorna true
			return true;
    	}
	//bool
	}
	else if(strcmp(tipo,"bool")==0){     
		// usa dirretamente funzione int
        return false;
    }
	// free(valore);
    return true;
}


_Bool controllaLongShort_tre_sessione(char** parti_var) {
    return (
		// casi con "long" prima
    	strcmp(parti_var[0], "long") == 0 && (
    	// "long long a"
    	(strcmp(parti_var[1], "long") == 0 && controllaNome(parti_var[2])) ||
    	// "long int a"
    	(strcmp(parti_var[1], "int") == 0 && controllaNome(parti_var[2])) ||
    	// "long double a"
        (strcmp(parti_var[1], "double") == 0 && controllaNome(parti_var[2]))
    	) ||

    	// "unsigned long a"
    	(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && controllaNome(parti_var[2])) ||
    	// "signed long a"
		(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && controllaNome(parti_var[2])) ||

    	// casi con "short" prima
    	strcmp(parti_var[0], "short") == 0 && (
    	// "short int a"
        (strcmp(parti_var[1], "int") == 0 && controllaNome(parti_var[2]))) ||

    	// "unsigned short a"
    	(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "short") == 0 && controllaNome(parti_var[2])) ||
    	// "signed short a"
    	(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "short") == 0 && controllaNome(parti_var[2])) ||

		// casi con char
		(strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "char") == 0 && controllaNome(parti_var[2])) ||
		// "signed char a"
    	(strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "char") == 0 && controllaNome(parti_var[2]))
	);
}

_Bool controllaLongShort_quattro_sessione(char** parti_var) {
	return (
        // "long long int a"
        (strcmp(parti_var[0], "long") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 && controllaNome(parti_var[3])) ||
        // "long long a" con unsigned/signed
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 && controllaNome(parti_var[3])) ||
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 && controllaNome(parti_var[3])) ||
        // "unsigned long int a"
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 && controllaNome(parti_var[3])) ||
        // "signed long int a"
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 && controllaNome(parti_var[3])) ||
        // "unsigned short int a"
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "short") == 0 && strcmp(parti_var[2], "int") == 0 && controllaNome(parti_var[3])) ||
        // "signed short int a"
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "short") == 0 && strcmp(parti_var[2], "int") == 0 && controllaNome(parti_var[3])) ||
        // "long int a" con signed
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "int") == 0 && controllaNome(parti_var[3]))
	);
}

//max numero di tipo dic
_Bool controllaLongShort_cinque_sessione(char** parti_var) {
    return (
        // "unsigned long long int a"
        (strcmp(parti_var[0], "unsigned") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 && strcmp(parti_var[3], "int") == 0 && controllaNome(parti_var[4])) ||
        // "signed long long int a"
        (strcmp(parti_var[0], "signed") == 0 && strcmp(parti_var[1], "long") == 0 && strcmp(parti_var[2], "long") == 0 && strcmp(parti_var[3], "int") == 0 && controllaNome(parti_var[4]))
    );
}


/*controlla se la dichiarazione della variabile è valida
true per:
1. '[tipo] a'
2. '[tipo] a = [valore]'
3. '[storage] [tipo] a = [valore]'
4. '[qualificatore] [tipo] a = [valore]'
5. '[storage] [qualificatore] [tipo] a = [valore]
*/
bool controllaDichiarazioneVariabile(char* str, List* strutture_definite){
	if (!str) return false;

	//duplica e splitta 
	int n_parti;
	char* copia_str=strdup(str);	//"int a = 10"
	//printf("\n %s \n",parole_copia);
    char** parti_var=split_variabile(copia_str,&n_parti);	// "int", "a", "10"
	if (!parti_var)
	{
		free(copia_str);
		return false;
	}
	bool risultato = false;
	
	//caso: "int"
	if(n_parti<=1){
		risultato = false; //int 
	}
	//caso: "int a"
	else if(n_parti==2){ // "int i"
		risultato = (controlloTipo(parti_var[0]) && controllaNome(parti_var[1]));
		//todo: controlla il tipo pexr al struttura definita in controllotipo

	}
	//caso: "int a = 10"
	else if(n_parti==3){	//"int i = 10"
		if (controlloTipo(parti_var[0]) && controllaNome(parti_var[1])){
			if (controlloValoreAssegnato(parti_var[2], parti_var[0])){
				risultato = true;
			}
			else{
				printf("[controllaDichiarazioneVariabile] valore '%s' non valido per tipo '%s'\n", parti_var[2],parti_var[0]);
			}
		}
		else if(controllaLongShort_tre_sessione(parti_var))
    	{
			printf("ppppppppppppppppppppppppppppppppppp\n");
			risultato=true;
		}
	}else if(n_parti==4){	//"short int a=10" "long int a=10" "unsigned int a=10"  "unsigned short"
		if(ricerca(str,"=")){
			risultato=(controllaLongShort_tre_sessione(parti_var));
		}
		else{
			risultato=controllaLongShort_quattro_sessione(parti_var);
		}
	}else if(n_parti == 5){	//"unsigned short int a= 10" "long long int a=10"
		if(ricerca(str,"=")){
			risultato = controllaLongShort_quattro_sessione(parti_var);
		}
		else{
			risultato = controllaLongShort_cinque_sessione(parti_var);
		}
	}else if(n_parti == 6){	//"unsigned long long int a =10"
		if(ricerca(str,"=")){
			risultato = controllaLongShort_cinque_sessione(parti_var);
		}
		else{
			risultato = 0;
		}
	}
	
    free(parti_var);
    free(copia_str);
	return risultato;
}

/*
controlla se la riga corrente è una valida dichiarazione del main().
-riga: riga attuale
ritorna true se è una valida dichiarazione di main. es: "int main (int argc, char* argv[])"
*/
bool isMain(char* riga) {
	if(riga == NULL) return false;

	//trova parentesi e valida che sono nell'ordine corretto
	char* p_open = strchr(riga, '(');
	char* p_close = strchr(riga, ')');
	if (p_open == NULL || p_close == NULL || p_open >= p_close){
		return false;
	}

	//estrai parte prima di '(': "int main"
	int len_prima = p_open - riga;
	char* prima_parte_str = strndup(riga, len_prima);
	if (!prima_parte_str) return false;

	//processa prima parte
	char* prima_parte_pulito = eliminaSpaziDxSx(prima_parte_str);
	int parole_prima_parte;
	char** tokens_primaparte = split(prima_parte_pulito, " \t", &parole_prima_parte);
	bool ok_segnatura = (parole_prima_parte == 2 &&
							strcmp(tokens_primaparte[0],"int") == 0 &&
							strcmp(tokens_primaparte[1], "main") == 0);
	free(tokens_primaparte);
	free(prima_parte_str);
	if(!ok_segnatura) return false;

	//estrai gli argomenti/contenuto tra le parentesi
	int len_args = p_close - p_open - 1;
	char* args= NULL;
	if (len_args>0) args = strndup(p_open+1, len_args);	//copia argomenti
	//se len_args==0, significa che è stato dichiarato 'int main()'
	bool ok_args=false;

	if (args==NULL || strlen(args) == 0){		//strlen considera 0 una serie di spazi bianchi(?)
		//caso: 'main()' oppure 'main(				)'
		ok_args = true; 
	}
	else{
		//casi: 'int main(int <nome>, char* <nome>[])'	... 
		ok_args = controlloArgomentiMain(args);
	}

	free(args);
	return ok_args;
}


/*
controlla se la dichiarazione di if/else/while/for sia corretta
token deve non avere spazi a sx.
- token: stringa in forma "if (zh给)..."
*/
bool controllaStrutturaControllo(char* token, int riga, Statistiche* stats){
	if (!token) return false;

	const char* kw_controllo[] = {"if", "else", "for", "while", NULL};
	bool kw_valido = false;
	const char* kw_trovata = NULL;
	
	//controllo dichiarazione corretta kw
	char* copia_token = strdup(token);
	int n_str;
	char** stringhe = split(copia_token, " {(",&n_str);	//splitta su 'if() -> if\0)', else{} -> else\0}, if (...) -> if\0(...)
	if (n_str == 0){
		free(stringhe);
		free(copia_token);
		return false;
	}
	char* kw = stringhe[0];
	for(int i = 0; kw_controllo[i] != NULL; i++){
		if (strcmp(kw, kw_controllo[i])== 0){
			kw_valido = true;
			kw_trovata = kw_controllo[i];
			break;
		}
	}
	//pulizia se kw non valido
	if (!kw_valido){
		free(copia_token);
		return false;
	} 

	//se kw è else, ritorna true
	if(strcmp(kw_trovata,"else") == 0){
		free(copia_token);
		printf("[ControlloVariabile] struttura '%s' valida alla riga %i\n", kw_trovata, riga);
		return true;
	}

	//cerca le parentesi tonde e verifica se è corretto
	char* paren_aperta = strchr(token, '(');
	char* paren_chiusa = strchr(token, ')');
	bool ok_parentesi = (paren_aperta && paren_chiusa && paren_aperta < paren_chiusa);
	if(!ok_parentesi){
		printf("[ControlloVariabile] ERRORE: '%s' senza parentesi valide alla riga %i\n", kw_trovata, riga);
		stats->errori_rilevati++;
		return false;
	}
	else{
		printf("[ControlloVariabile] struttura '%s' valida alla riga %i\n", kw_trovata, riga);
	}

	free(copia_token);
	return ok_parentesi;
}	

/*
la funzione controlla la correttezza di:
1. dichiarazione del main + argomenti
2. dichiarazione delle strutture di controllo, es: if, else, for, while
3. dichiarazione delle variabili

*/
void check_file(char* filename, Statistiche* stats, List* variabili){
	//apertura file e gestione errore
	FILE* file = fopen(filename, "r");
	if (!file){
		printf("[ControlloVariabile] errore apertura file: %s\n", filename);
		return;
	}

	//init variabili utili
	char riga[1024];
	int n_riga = 1;
	int n_graffe = 0;
	bool main_dichiarato = false;
	bool in_commento_multiplo = false;
	bool in_struct = false;
	List* struct_definite = list_create();

	//scorre le righe del file
	while (fgets(riga, sizeof(riga), file) != NULL){
		//rimpiazza \n con \0
		riga[strcspn(riga, "\n")] = '\0';

		//salta commenti se stiamo in un commento multilinea
		if(in_commento_multiplo){
			if (strstr(riga, "*/") != NULL){	//se è stato chiuso il commento resetta flag
				in_commento_multiplo = false;
			}
			n_riga++;	
			continue;	//salta riga corrente
		}

		//rimuovi spazi dx e sx, elimina commenti inline del tipi "int a = 0; //commento"
		rimuoviCommentoInline(riga);
		char* clean = eliminaSpaziDxSx_v2(riga);
		
		//salta righe vuote
		if (strlen(clean) == 0){
			n_riga ++;
			continue;
		}

		//salta le righe dopo  commenti del tipo ' /* .... */ '
		if (strlen(clean) >= 2 && clean[0] == '/' && clean[1] == '*' ){
			in_commento_multiplo = true;
			n_riga++;
			continue; 	//salta la riga corrente
		}

		//conta graffe della riga
		for(int k = 0; k < strlen(clean); k++){
			if (clean[k] == '{') n_graffe++;
			else if(clean[k] == '}') n_graffe--;
		}

		//controlla for , prima dello split su '{};'.
		//altrimenti esplode su 'for(int i=0; i< qualcosa; i++){...}'
		if (strncmp(clean, "for", 3) == 0){
			if(controllaStrutturaControllo(clean, n_riga, stats)){
				//se valido salta
			}
			else{
				stats->errori_rilevati++;
			}
			free(clean);
			n_riga++;
			continue;
		}


		//split su '{' '}' e ';', controllo split vuoto.
		//tokens ha: "int test = 0", "char hello, ...", array di stringhe.
		int n_tokens;
		char** tokens = split(clean, "{};", &n_tokens);
		if(!tokens || n_tokens == 0){
			free(clean);
			free(tokens);
			n_riga++;
			continue;
		}

		//salta commenti e include
		if(controllaRigaCommento(tokens[0]) || controllaRigaInclude(tokens[0])){
			n_riga ++;
			continue;
		}

		//processa ogni token
		for(int i = 0; i < n_tokens; i++){
			// un token è del tipo: "int test = 0"
			char* token = tokens[i];
			if(!token) continue;
			char* pulito = eliminaSpaziDxSx_v2(token);
			if (!pulito) continue;

			//gestione struct
			if (!in_struct && isStruct(pulito)){
				in_struct = true;

			}
		
			//controllo dichiarazione main() e gestione main() duplicato
			if (!main_dichiarato && isMain(pulito)){
				printf("[ControlloVariabile] dichiarazione main() corretta alla riga: %i\n", n_riga);
				main_dichiarato = true;
				free(pulito);
				continue;
			}
			else if(main_dichiarato && isMain(pulito)){
				stats -> errori_rilevati++;
				printf("[ControlloVariabile] dichiarato un altro main alla riga: %i\n", n_riga);
				free(pulito);
				continue;
			}

			//controlla se la dichiarazione del return è valido
			if(strncmp(pulito, "return", 6) == 0){
				// printf("[ControlloVariabile] attualmente nel controllo del return, pulito = '%s' \n", pulito);
				if (!main_dichiarato){
					printf("[ControlloVariabile] return fuori dal main alla riga %i \n", n_riga);
					stats->errori_rilevati++;
				}
				else if (controllaReturnValido(pulito, variabili)){
					printf("[ControlloVariabile] return ok alla riga: %i \n", n_riga);
				}
				else{
					printf("[ControlloVariabile] return non valido alla riga: %i \n", n_riga);
				}
				free(pulito);
				continue;
			}

			//controllo della dichiarazione if/else/for/while
			if(controllaStrutturaControllo(pulito, n_riga, stats)){
				free(pulito);
				continue;
			}

			//controllo assegnazione, es: 'a = b + 10;'
			if (isAssegnazioneValida(pulito, variabili)){
				printf("[ControlloVariabile] assegnazione valida alla riga: %i \n", n_riga);
				free(pulito);
 				continue;
			}
			// else{
			// 	printf("[ControlloVariabile] assegnazione non valida alla riga: %i \n", n_riga);
			// 	free(pulito);
			// 	continue;
			// }

			//controllo dichiarazione delle variabili
			if(controllaDichiarazioneVariabile(pulito)){
				// Variabile
				stats->variabili_controllate++;	//conta solo dichiarazione valida di var
				printf("[ControlloVariabile] variabile valida '%s' alla riga %i\n", pulito, n_riga);
			}
			else{
				stats->errori_rilevati++; //errori contati separatamente
				printf("[ControlloVariabile] variabile non valida '%s' alla riga %i\n", pulito, n_riga);
			}

			//altri controlli andranno qui

			free(pulito);
		}

		free(tokens);
		free(clean);
		n_riga++;
	}

	fclose(file);

	//controlla che il numero delle graffe sia corretto
	if (n_graffe != 0) {
		printf("[ControlloVariabile] ATTENZIONE: parentesi graffe non bilanciate (differenza: %i)\n", n_graffe);
		stats->errori_rilevati++;
	}
	
	if(!main_dichiarato){
		printf("[ControlloVariabile] ATTENZIONE: manca la dichiarazione del main()\n");
		stats->errori_rilevati++;
	}

	return;
}

//funziona ritorna una lista di tutte le variabili dichiarate.
//setta flag di ogni variabile se è stata utilizzata o non.
List* controllaUtilizzoVariabili(char *nome_file_in, Statistiche *stats){
	List* variabili = list_create();	//lista per tenere traccia le variabili

	FILE* file = fopen(nome_file_in, "r");
	if (file == NULL) {
		printf("[ControlloVariabile] controlla_var_inutilizzate: errore di apertura file\n");
		return NULL;
	}

	char riga[512]; //buffer per leggere una riga alla volta
	int riga_attuale = 0;

	//ciclo principale che scorre le righe del file
	//nota: fgets(char *str, int n, FILE *stream):
	//	str: buffer dove viene salvata la stringa letta
	//	n: numero massimo di caratteri da leggere
	//	stream: sorgente da cui leggere
	//
	//si ferma sempre al \n, che viene incluso.
	//aggiunge il terminatore \0 alla fine della riga
	//restituisce str in caso di successo, NULL in caso di errore o fine file.
	while (fgets(riga, sizeof(riga), file) != NULL) {	
		riga_attuale ++;
		//pulizia riga
		riga[strcspn(riga, "\n")] = '\0';	//rimuove \n
		rimuoviCommentoInline(riga);
		char *riga_pulita = rimuoviSpaziSx(riga);

		//se la riga corrente è commento o #include o vuota, salta.
		if(controllaRigaCommento(riga_pulita) == true){
			//printf("[controllaVarInutilizzata]: commento  alla riga %i\n", riga_attuale);
			continue;
		}
		else if(controllaRigaInclude(riga_pulita) == true){
			//printf("[controllaVarInutilizzata]: include  alla riga %i\n", riga_attuale);
			continue;
		}
		else if(controllaRigaVuota(riga_pulita) == true){
			//printf("[controllaVarInutilizzata]: riga %i vuota\n", riga_attuale);
			continue;
		}

		//ottieni i token per la riga
		int numero_token;
		char** tokens = split(riga_pulita, " \t;", &numero_token);

		/*
		for(int i = 0; i< numero_token; i++){
			printf("[TOKENS] : %s \n", tokens[i]);
		}
		*/
		
		//controlla se è una dichiarazione valida di variabile, e lo mette nella lista variabili
		for (int i = 0; i < numero_token; i++){	//per ogni token
			//controlla se tokens[i] è un tipo
			for (int t = 0; tipi_base[t]; t++){	//per ogni tipo
				if (strcmp(tokens[i], tipi_base[t]) == 0 && i+1 < numero_token){
					//tronca il nome dell variabile al primo carattere non alfanum (es. =, ;, [)
					char *candidato = pulisciNomeVariabile(tokens[i+1]);

					//controlla se il token corrente è una funzione
					//printf("[DEBUG] CANDIDATO var CORRENTE : %s\n", candidato);
					if (strchr(tokens[i+1], '(') != NULL){
    					free(candidato);
						//printf("funzione rilevata alla riga %i\n", riga_attuale);
   					 	break;  // è una funzione, salta
					}


					//filtra puntatori, array, main
					if(candidato[0] != '*' && candidato[0] != '[' && strcmp(candidato, "main") != 0 && isalpha(candidato[0])){
						Variabile* var = malloc(sizeof(Variabile));
						var->tipo = strdup(tipi_base[t]);
						var->nome = strdup(candidato);
						var->riga_dichiarata = riga_attuale;
						var->usata = false;

						list_append(variabili, var);
						printf("[ControllaVarInutilizzate] dichiarata '%s' alla riga %d con tipo '%s': \n", var->nome, var->riga_dichiarata, var->tipo);
					}
					break; //una dichiarazione alla volta
				}
			}
		}
	} 


	//secondo passaggio per vedere l'utilizzo
	rewind(file);
	riga_attuale = 0;

	while (fgets(riga, sizeof(riga), file)) {
		riga_attuale++;
		riga[strcspn(riga, "\n")] = '\0';
		char *riga_pulita = rimuoviSpaziSx(riga);
		//se la riga corrente è commento o #include o vuota, salta.
		if (controllaRigaCommento(riga_pulita) == true || controllaRigaInclude(riga_pulita) == true || controllaRigaVuota(riga_pulita) == true){
			continue;
		}

		int numero_token;
		char **tokens = split(riga_pulita, " \t=;,()[]{}+-*/<>!&|", &numero_token);

			//per ogni riga scorre una volta le variabili della lista
		for (int i = 0; i < variabili->numero_elementi_attuali; i++)
		{
			Variabile *var_att = list_get(variabili, i);
			if (var_att->usata == true){
				continue;
			}
			//scorre i token della riga attuale
			for (int t = 0; t < numero_token; t++){
				char *token_pulito = pulisciNomeVariabile(tokens[t]);
				if (strcmp(token_pulito, var_att->nome) == 0 && riga_attuale != var_att->riga_dichiarata){
					//se il token è uguale al nome della variabile e la riga attuale non è la riga in cui la var è stata dichiarata
					var_att->usata = true;	//allora la var è stata utilizzata
					printf("variabile '%s' utilizzata alla riga %i\n", var_att->nome, riga_attuale);
				}
				free(token_pulito);
			}
		}
		free(tokens);
	}
	fclose(file);
	return variabili;
}









