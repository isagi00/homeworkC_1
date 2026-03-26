#include <stdio.h>
#include <supporto.h>
char[] controlloVariabile(char[] filename){
	FILE *file = fopen(filename,"r");
	int numero;

	if(file == Null) {
		return "Erroew apertura file\n";
	}

	char riga[100];
	while (fgets(riga, sizeof(riga),file) != Null) {

		char **p=split(riga,&numero);
		if(p[0][0]=="#" || (p[0][0]=="/" && p[0][1]=="/") || ){
			continue;
		}
		else{
			if(strcmp(p[1],"main")==0){
				continue;
}
		}
	}

}


