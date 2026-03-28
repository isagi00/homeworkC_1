#include "../supporto.h"
#include <stdio.h>

int main(){
    char str[64] = "#include <stdio.h>";
    printf("#include: %s\n",controllaRigaInclude(str) ? "true" : "false");

    char str2[64] = "#       include <stdio.h>";
    printf("#   include: %s\n",controllaRigaInclude(str2) ? "true" : "false");

    char str3[64] = "#     includeinclude <stdio.h>";
    printf("#includeinclude <>: %s\n",controllaRigaInclude(str3) ? "true" : "false");


    return 0;

}