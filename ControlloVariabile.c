#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "supporto.h"


const char* tipi_base[] = {"int", "float", "double", "char", "long", "bool", NULL};

void stampaParoleSplit(char **parole, int numero_parole) {
    printf("Contenuto di parole_split_pv (numero_parole = %d):\n", numero_parole);
    for (int i = 0; i < numero_parole; i++) {
        printf("[%d]: '%s'\n", i, parole[i]);
    }
}

bool isMain(char* riga) {
    int numeroParola;
    char** parola = split(riga, "()", &numeroParola);
    
    if(numeroParola < 1){
        free(parola);
        return false;
		//palle
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
	printf("[Errore tipo]");
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

bool controlloNome(char* parola){

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

bool controlloCorrettezzaVariabile(char* valore,char* tipo){
	
	if(strcmp(tipo,"char")==0){      
        
		if(strlen(valore)<=3 && strlen(valore)>=2 && valore[0]=='\'' && valore[strlen(valore)-1]=='\''){
			return true;
		}
        return false;
    }else if(strcmp(tipo,"int")==0 || strcmp(tipo,"long")==0 || strcmp(tipo,"short")==0){ 

        for(int i = 1; i <strlen(valore); i++){
	
            if(!(isalnum(valore[i]) ||valore[i]=='+'||valore[i]=='-'||valore[i])==' '|| valore[i]=='\t'){
                return false;
            }
        }
        return true;

    }else if(strcmp(tipo,"float")==0 || strcmp(tipo,"double")==0){
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
    free(valore);
    return true;

	}else if(strcmp(tipo,"bool")==0){     
        if(strcmp(valore,"true")==0  || strcmp(valore,"false")==0 || strcmp(valore,"1")==0 || strcmp(valore,"0")==0){
            return true;
        }
        return false;
    }
	
}

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
	}else if(numero_parte==2){ // int i
		if(controlloTipo(parte_variabile[0])){
			if(controlloNome(parte_variabile[1])){
				return true;
			}
			else{
				return false;
			}
		}
		else{
			return false;
		}
	}else if(numero_parte==3){
		if(controlloTipo(parte_variabile[0]) && controlloNome(parte_variabile[1])){
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

//controlla il tipo delle variabili
char* controlloVariabile(char* filename, Statistiche *stats){
    FILE *file = fopen(filename, "r");
    if(file == NULL){
        printf("[ControlloVariabile] errore apertura file\n");
        return NULL;
    }

    char riga[128];
    int numero_pv, numero_s;
    int numeroRiga = 0;
    bool esisteMain = false;

    while(fgets(riga, sizeof(riga), file) != NULL){
        riga[strcspn(riga, "\n")] = '\0';

        char *rigaPul = rimuoviSpaziSx(riga);
        char **parole_split_pv = split(rigaPul, "{};", &numero_pv);
		//stampaParoleSplit(parole_split_pv,numero_pv);
        if(numero_pv == 0){
            free(parole_split_pv);
            numeroRiga++;
            continue;
        }

        if(controllaRigaCommento(parole_split_pv[0]) || controllaRigaInclude(parole_split_pv[0])  || strcmp(parole_split_pv[0], "{") == 0 || strcmp(parole_split_pv[0], "}") == 0){
            numeroRiga++;
            free(parole_split_pv);
            continue;
        }
		
        for(int i = 0; i < numero_pv; i++){
            char *cp = parole_split_pv[i];
			char *cp1 = strdup(rimuoviSpaziSx(cp));
			char** cp2=NULL;
			int numero_variabile;
            //char **parole_split_s = split(cp, " ", &numero_s);

			//printf("%s\n",cp);
            if(isMain(cp)){
				if(!esisteMain){
					printf("Questa è main riga:%i\n",numeroRiga);
               		esisteMain = true;
					continue;
				}
				else{
					stats->errori_rilevati++;
					printf("[ERRORE MAIN]:doppia main riga %i\n",numeroRiga);
                	continue;
            	}
			}else if(isFunzione(cp1)){
				printf("è una funzione %d riga: %i\n",isFunzione(cp1),numeroRiga);
			}else{
				//printf("%s è una funzione %d riga: %i\n",cp1,isFunzione(cp1),numeroRiga);
				if(controllaDichiarazioneVariabile(cp1)){
					printf("(Dichiarazione Variabile) riga: %i\n",numeroRiga);
				}
				else{
					printf("[Errore Variabile] %s riga: %i\n",cp1,numeroRiga);
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
						haNomeCorretto=controlloNome(parole_split_s[j]);
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
                    haNomeCorretto = controlloNome(parole_split_s[j]);
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




/*
		char **parole = split(riga," \t", &numero_parole_riga_corrente);

		//debug
		printf("[DEBUG] riga %d\n", numeroRiga);
		for (int i = 0; i < numero_parole_riga_corrente; i++){
			printf("stringa:[%s] \n", parole[i]);  // stampa ogni stringa tra virgolette
		}

		
		//controllo riga vuota
		if (numero_parole_riga_corrente == 0) {
            printf("[ControlloVariabile] riga %d vuota, skip\n", numeroRiga);
            free(parole);
            numeroRiga++;
            continue;  // salta alla prossima riga
        }

		//controlla se è #include o //commento
		if (parole[0][0] == '#' || (parole[0][0] == '/' && strlen(parole[0]) > 1 && parole[0][1] == '/')) {
            printf("[ControlloVariabile] riga %d: commento/include, skip\n", numeroRiga);
            free(parole);
            numeroRiga++;
            continue;  // salta alla prossima riga
        }
			
		//controllo tipi variabile
		if((strcmp(parole[0],"int") == 0 || strcmp(parole[0],"long") == 0 || strcmp(parole[0], "short") == 0 )){
            printf("[ControlloVariabile] rilevato tipo intero alla riga %i\n", numeroRiga);
			variabili_controllate += 1;
		}
		else if(strcmp(parole[0],"char")==0){
			printf("[ControlloVariabile] rilevato tipo char alla riga %i\n", numeroRiga);
			variabili_controllate += 1;
		}
        else if(strcmp(parole[0],"float")==0){
            printf("[ControlloVariabile] rilevato tipo float alla riga %i\n", numeroRiga); 
			variabili_controllate += 1;          
		}
		else if(strcmp(parole[0],"bool")==0){
            printf("[ControlloVariabile] rilevato tipo bool alla riga %i\n", numeroRiga);         
			variabili_controllate += 1;                      
		}
		else{
			printf("[ControlloVariabile] rilevato errore di tipo alla riga %i\n", numeroRiga);
			errori_rilevati += 1;
		}
		//pulizia fine riga
        numeroRiga++; //passa riga successiva
		free(parole);	//libera spazio riservato dell'array parole   
		}
	*/
	fclose(file);
	printf("[ControlloVariabile] termine controllo variabili\n");
	return NULL;
}


//controlla variabili inutilizzate
List* controllaVarInutilizzate(char *nome_file_in, Statistiche *stats){
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
			printf("[controllaVarInutilizzata]: commento  alla riga %i\n", riga_attuale);
			continue;
		}
		else if(controllaRigaInclude(riga_pulita) == true){
			printf("[controllaVarInutilizzata]: include  alla riga %i\n", riga_attuale);
			continue;
		}
		else if(controllaRigaVuota(riga_pulita) == true){
			printf("[controllaVarInutilizzata]: riga %i vuota\n", riga_attuale);
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
						printf("funzione rilevata alla riga %i\n", riga_attuale);
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


	//secondo passaggio 
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
					printf("variabile %s utilizzata alla riga %i\n", var_att->nome, riga_attuale);
				}
				free(token_pulito);
			}
		}
		free(tokens);
	}

		//controllo finale delle variabili non utilizzate
		for (int i = 0; i < variabili->numero_elementi_attuali; i++){
			Variabile *var = list_get(variabili, i);
			if (var->usata == false){
				printf("[ControllaVarInutilizzate] variabile %s non usata\n", var->nome);
				stats->variabili_inutilizzate++;
			}
		}
	fclose(file);
	return variabili;

}








