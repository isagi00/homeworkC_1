
#include <stdio.h>

char[] controlloVariabile(char[] filename){
	FILE *file = fopen(filename,"r");
	
	if(file == Null) {
		return "Erroew apertura file\n";
	}

	char riga[100];
	while (fgets(riga, sizeof(riga),file) != Null) {
		if(riga[0]=="#"):
			continue;
		if(riga[0]=="/" and riga[1]=="/"):
			continue;
		
	}

}


