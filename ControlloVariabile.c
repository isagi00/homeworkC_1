#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "supporto.h"


const char* tipi_base[] = {"int", "float", "double", "char", "long", "bool", NULL};

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

bool controllaNome(char* parola){

    int n = strlen(parola);
    
    for(int i = 0; i < n; i++){
        if(i == 0){
            if(!(isalpha(parola[i]) || parola[i] == '_')){
                return false;
            }
        }
        else if(!(isalnum(parola[i]) || parola[i] == '_')){
            return false;
        }
    }
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




bool controlloCorrettezzaVariabile(char* valore,char* tipo){
	//correttezza variabile char
	if(strcmp(tipo,"char")==0){      
        
		if(strlen(valore)<=3 && strlen(valore)>=2 && valore[0]=='\'' && valore[strlen(valore)-1]=='\''){
			return true;
		}
        return false;
    }
	//correttezza variabile int, long, short
	else if(strcmp(tipo,"int")==0 || strcmp(tipo,"long")==0 || strcmp(tipo,"short")==0){ 

        for(int i = 1; i <strlen(valore); i++){
	
            if( !(isalnum(valore[i]) ||valore[i]=='+'||valore[i]=='-'||valore[i]==' '|| valore[i]=='\t') ){
                return false;
            }
        }
        return true;
    }
	//correttezza variabile float o double
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
    	}
	//correttezza bool
	}else if(strcmp(tipo,"bool")==0){     
        if(strcmp(valore,"true")==0  || strcmp(valore,"false")==0 || strcmp(valore,"1")==0 || strcmp(valore,"0")==0){
            return true;
        }
        return false;
    }
	free(valore);
    return true;
}

//controlla se la dichiarazione della variabile è valida
bool controllaDichiarazioneVariabile(char* parola){
	int numero_parte;
	char* parole_copia=strdup(parola);
	//printf("\n %s \n",parole_copia);
    char** parte_variabile=split_variabile(parole_copia,&numero_parte);
	int pppp=0;
	/*while (parte_variabile[pppp]!=NULL){
		printf("%s\n",parte_variabile[pppp]);
		pppp++;
	}
*/
	
	if(numero_parte<=1){
		return false; //int 
	}
	else if(numero_parte==2){ // int i
		if(controlloTipo(parte_variabile[0])){
			if(controllaNome(parte_variabile[1])){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}
	else if(numero_parte==3){
		if(controlloTipo(parte_variabile[0]) && controllaNome(parte_variabile[1])){
			if(controlloCorrettezzaVariabile(parte_variabile[2],parte_variabile[0])){
				return true;
			}else{
	////mmmmmmmmmmmmmmmmm
			}
		}
		else{
			return false;
		}
	}else{
		return false;
	}

    free(parte_variabile);
    free(parole_copia);
	return false;
}

//controlla se dichiarazione main valida
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

//controlla il tipo delle variabili
char* controlloVariabile(char* filename, Statistiche *stats){
    FILE *file = fopen(filename, "r");
	//gestione errore apertura file
    if(file == NULL){
        printf("[ControlloVariabile] errore apertura file\n");
        return NULL;
    }

    char riga[128];
    int numero_pv, numero_s;
    int numeroRiga = 1;	//riga corrente
    bool esisteMain = false;

    while(fgets(riga, sizeof(riga), file) != NULL){
        riga[strcspn(riga, "\n")] = '\0';

        char *rigaPul = rimuoviSpaziSx(riga);
        char **parole_split_pv = split(rigaPul, "{};", &numero_pv);
		//stampaParoleSplit(parole_split_pv,numero_pv);

		//controlla se è una riga vuota
        if(numero_pv == 0){
            free(parole_split_pv);
            numeroRiga++;
            continue;
        }
		//controlla se è commento o include 
        if(controllaRigaCommento(parole_split_pv[0]) || controllaRigaInclude(parole_split_pv[0])){
            numeroRiga++;
            free(parole_split_pv);
            continue;
        }
		
		//controlla se la riga attuale è una dichiarazione valida di main, funzione o di variabile
        for(int i = 0; i < numero_pv; i++){
            char *cp = parole_split_pv[i];
			char *cp1 = strdup(rimuoviSpaziSx(cp));
			char** cp2=NULL;
			int numero_variabile;
            //char **parole_split_s = split(cp, " ", &numero_s);

			//printf("%s\n",cp);

			//controlla se è una dichirazione valida di main
            if(isMain(cp)){
				if(!esisteMain){
					printf("[ControlloVariabile] dichiarazione del main() corretta:%i\n",numeroRiga);
               		esisteMain = true;
					continue;
				}
				else{
					stats->errori_rilevati++;
					printf("[ControlloVariabile]: rilevato un secondo main alla riga:  %i\n",numeroRiga);
                	continue;
            	}
			}
			//controlla se è una dichiarazione valida di funzione
			else if(isFunzione(cp1)){
				printf("[ControlloVariabile] rilevata una funzione %d riga: %i\n",isFunzione(cp1),numeroRiga);
			}
			//controlla se è una dichiarazione valida di variabile
			else{
				//printf("%s è una funzione %d riga: %i\n",cp1,isFunzione(cp1),numeroRiga);
				if(controllaDichiarazioneVariabile(cp1)){
					stats->variabili_controllate++;
					printf("[ControlloVariabile] dichiarata variabile '%s' alla riga: %i\n", cp1, numeroRiga);
				}
				else{
					stats->errori_rilevati++;
					printf("[Errore Variabile] errore di dichiarazione della variabile '%s' alla riga: %i\n",cp1,numeroRiga);
					
				}
			}
			
			/*else{
				
			}
            
            

            char* tipo_ = NULL;
            bool haTipo = false;
            bool valoreCor = false;
			bool isFunzione = false;
            RisultatoNome haNomeCorretto = {false, false};
			
			
            for(int j = 0; j < numero_s; j++){
                if(!haTipo){
                    if(controlloTipo(parole_split_s[j])){
                        haTipo = true;
                        tipo_ = parole_split_s[j];
                    } else {
						haNomeCorretto=controllaNome(parole_split_s[j]);
						int n_separa_uguale;
						char** separa_uguale = split(parole_split_pv[i], "=", &n_separa_uguale);
						if(haNomeCorretto.cor && n_separa_uguale==2){
							int n_separa_uguale_spazzio;
							char** separa_uguale_spazzio = split(separa_uguale[0], " ", &n_separa_uguale_spazzio);
							if (n_separa_uguale_spazzio>1)
							{
								stats->tipi_dato_scorretti++;
								stats->errori_rilevati;
								printf("[ERRORE riga %d] tipo scorretto: %s\n", numeroRiga, parole_split_s[j]);
							}
							else
							{
								printf("[è una variabile\n]");
								break;
							}
						}
						else{
 							printf("errore forma");
						}
                    }
                } else {
                    haNomeCorretto = controllaNome(parole_split_s[j]);
                    if(!haNomeCorretto.cor){                          
                        stats->nomi_variabili_non_corretti++;
						stats->errori_rilevati++;
						haTipo=false;
						printf("[ERRORE riga %d] nome scorretto: %s\n", numeroRiga, parole_split_s[j]);
                    }
                    if(haNomeCorretto.haugu){
                        valoreCor = controllaVar(parole_split_s[j], tipo_); 
                        if(!valoreCor){
                            stats->errori_rilevati++;
							printf("[ERRORE riga %d] valore scorretto: %s\n", numeroRiga, parole_split_s[j]);
                        }
                    }
                }
            }*/

            //free(parole_split_s);
        }
        numeroRiga++;
        free(parole_split_pv);
    }
	fclose(file);
	printf("[ControlloVariabile] termine controllo variabili\n");
	return NULL;
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
						var->nome = strdup(candidato);
						var->riga_dichiarata = riga_attuale;
						var->usata = false;

						list_append(variabili, var);
						printf("[ControllaVarInutilizzate] dichiarata '%s' alla riga %d\n", var->nome, var->riga_dichiarata);
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









